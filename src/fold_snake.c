#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "config.h"

struct globalArgs_t {
  int output;                 /* -o option */
  char *outFileName;
  FILE *outFile;
  int specular;               /* -s option */
  char *energyFileName;
  FILE *energyFile;
  char *newickFileName;
  FILE *newickFile;
  int countOnly;              /* -c option */
  int boundingBox;            /* -b option */
  int maxSolutions;           /* -M option */
  int verbose;                /* -v option */
} globalArgs;

static const char *optString = "o:sE:M:n:cb:hv?";

struct coordinates_t {
  int x;
  int y;
  int z;
};

struct HamiltonianWalk_t {
  struct coordinates_t * coord;
  int length;
  int MAX_length;
  int last_element;
  int last_direction;
  int solutions;
  struct coordinates_t min;
  struct coordinates_t max;
} hm;
 

/* Headers */
/** Prints the Hamiltonian walk to a coordinates file */
void printCSV (int seq[], FILE * fh);
void print_hm ();
/** Counts the number of contacts in the structure */
int countContacts ();
/** Creates a node with coordinates and checks for size/overlap */
void createNode(int direction);
/** initialises first two elements of puzzle */
void init_hm ( int * seq, int volume );
void destroy_hm();
/** Checks if a number is a perfect cube */
int is_perfect_cube(int);
/** Checks if there was a z dimension step already */
int walked_in_z();
/** Prints help message */
void usage(char*);

/* Main */
int main ( int argc, char *argv[] )
{
  clock_t start = clock();
  /****************************/
  /* Parse cmd line arguments */
  /****************************/
  globalArgs.output = 1;             /* Output is default to stdout */
  globalArgs.outFileName = NULL;     /* Output file name */
  globalArgs.outFile = stdout;       /* Output FILE handle */
  globalArgs.specular = 0;           /* Print specular solutions */
  globalArgs.energyFileName = NULL;  /* Energy output file name */
  globalArgs.energyFile = NULL;      /* Energy output FILE handle */
  globalArgs.newickFileName = NULL;  /* Newick string output file name */
  globalArgs.newickFile = NULL;      /* Newick output FILE handle */
  globalArgs.countOnly = 0;          /* Counts solutions, no output */
  globalArgs.boundingBox = -1;       /* Limits the structure to a cube of this size
                                         0 = infinite
                                        -1 = cuberoot of length of sequence (cube)   */
  globalArgs.maxSolutions = 0;       /* Limits the number of solutions (0 = no limit) */
  globalArgs.verbose = 0;            /* Prints heaps of stuff */

  int index;

  opterr = 0;

  int c;
  while ((c = getopt (argc, argv, optString)) != -1)
  {
    char *ptr = NULL;
    switch (c)
    {
      case 'o':
        globalArgs.outFileName = optarg;
        break;
      case 's':
        globalArgs.specular = 1;
        break;
      case 'E':
        globalArgs.energyFileName = optarg;
        break;
      case 'c':
        globalArgs.countOnly = 1;
        break;
      case 'b':
        globalArgs.boundingBox = strtol( optarg, &ptr, 0);
        if ( !( globalArgs.boundingBox ) && !( strcmp( optarg, ptr ) ) )
        {
          fprintf(stderr, "Argument for option -b has to be a non-negative integer or 0\n");
          exit(EXIT_FAILURE);
        }
        if ( globalArgs.boundingBox < 0 )
        {
          fprintf(stderr, "Argument for option -b cannot be negative\n");
          exit(EXIT_FAILURE);
        }
        ptr = NULL;
        break;
      case 'M':
        globalArgs.maxSolutions = strtol( optarg, &ptr, 0);
        if ( !( globalArgs.maxSolutions ) && !( strcmp( optarg, ptr ) ) )
        {
          fprintf(stderr, "Argument for option -M has to be a non-negative integer or 0\n");
          exit(EXIT_FAILURE);
        }
        if ( globalArgs.maxSolutions < 0 )
        {
          fprintf(stderr, "Argument for option -M cannot be negative\n");
          exit(EXIT_FAILURE);
        }
        ptr = NULL;
        break;
      case 'n':
        globalArgs.newickFileName = optarg;
        break;
      case 'v':
        globalArgs.verbose = 1;
        break;
      case 'h':
        usage(argv[0]);
        exit(EXIT_SUCCESS);
      case '?':
        if (optopt == 'o')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'E')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'b')
          fprintf(stderr, "Option -%c requires an integer.\n", optopt);
        else if ( optopt == 'M')
          fprintf(stderr, "Option -%c requires an integer.\n", optopt);
        else if (isprint (optopt))
          fprintf(stderr, "Unknown option '-%c'.\n", optopt);
        else
          fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
        usage(argv[0]);
        exit(EXIT_FAILURE);
      default:
        abort();
      }
    }
  if ( globalArgs.countOnly ) // -c option overrides solutions output
    globalArgs.output = 0;

  for (index = optind; index < argc; index++)
  {
    fprintf (stderr, "Argument '%s' does not correspond to any options.\n", argv[index]);
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  /*********************/
  /* Declare variables */
  /*********************/

  int i = 0;
  int cubeSide = 0;
  int nbElements = sizeof (Sequence) / sizeof (int);
  int structureLength = 1;
  for (i = 0; i < nbElements; i++)
    structureLength += Sequence[i] - 1;

  if ( ! is_perfect_cube(structureLength) )
  {
    fprintf(stderr, "WARNING: The sequence provided cannot fold into a perfect cube\n");
    cubeSide = ceil( cbrt(structureLength) );
  }
  else
    cubeSide = (int) cbrt(structureLength);
  
  if ( globalArgs.boundingBox == -1 )
    globalArgs.boundingBox = cubeSide;
  else
    if ( globalArgs.boundingBox && pow( globalArgs.boundingBox, 3 ) < structureLength )
    {
      fprintf(stderr, "The sequence provided does not fit into the bounding box. "
                      "It has to be at least %d\n", cubeSide);
      exit(EXIT_FAILURE);
    }

  if ( globalArgs.energyFileName )
  {
    globalArgs.energyFile = fopen(globalArgs.energyFileName, "w");
    if ( ! globalArgs.energyFile )
    {
      fprintf(stderr, "Could not open %s to write\n", globalArgs.energyFileName);
      globalArgs.energyFileName = NULL;
      globalArgs.energyFile = NULL;
    }
  }

  if ( globalArgs.newickFileName )
  {
    globalArgs.newickFile = fopen(globalArgs.newickFileName, "w");
    if ( ! globalArgs.newickFile )
    {
      fprintf(stderr, "Could not open %s to write\n", globalArgs.newickFileName);
      globalArgs.newickFileName = NULL;
      globalArgs.newickFile = NULL;
    }
  }
  
  if ( globalArgs.verbose )
  {
    fprintf( stderr, "OPTIONS:\n");
    fprintf( stderr, " - Output solutions: %s\n", globalArgs.output?"YES":"NO");
    fprintf( stderr, " - Output file handle: %s\n", globalArgs.outFileName?globalArgs.outFileName:"stdout");
    fprintf( stderr, " - Find enantiomers: %s\n", globalArgs.specular?"YES":"NO");
    fprintf( stderr, " - Energy output file: %s\n", globalArgs.energyFileName?globalArgs.energyFileName:"NULL");
    fprintf( stderr, " - Energy file handle open? %s\n", globalArgs.energyFile?"YES":"NO");
    fprintf( stderr, " - Newick tree file: %s\n", globalArgs.newickFileName?globalArgs.newickFileName:"NULL");
    fprintf( stderr, " - Newick file handle open? %s\n", globalArgs.newickFile?"YES":"NO");
    fprintf( stderr, " - Only count: %s\n", globalArgs.countOnly?"YES":"NO");
    fprintf( stderr, " - Bounding box: %d\n", globalArgs.boundingBox);
    fprintf( stderr, " - Max solutions to return: %d\n\n", globalArgs.maxSolutions);
    fprintf( stderr, "Cube Side=%d\n", cubeSide);
    fprintf( stderr, "Bounding box=%d\n", globalArgs.boundingBox);
  }

  /*********************************************************************/
  /* Initialise the Hamiltonian walk structure with first two elements */
  /*********************************************************************/

  init_hm( Sequence, structureLength );

  /**************************/
  /* Enter recursive search */
  /**************************/

  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, "(" );
  createNode('X');
  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, "+x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / structureLength );
  createNode('x');
  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, "-x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / structureLength );
  createNode('Z');
  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, "+z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / structureLength );
    if ( globalArgs.specular )
      fprintf( globalArgs.newickFile, "," );
  if ( globalArgs.specular )
  {
    createNode('z');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "-z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / structureLength );
  }
  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, ");" );
  
  clock_t end = clock();
  fprintf (stderr, "Solutions found = %d\n", hm.solutions);

  fprintf(stderr, "Time elapsed = %d (%.3f secs)\n", (int) (end - start), (float) (end - start) / CLOCKS_PER_SEC);

  if ( globalArgs.energyFile )
    fclose ( globalArgs.energyFile );
  if ( globalArgs.newickFile )
    fclose ( globalArgs.newickFile );

  destroy_hm();
  exit(EXIT_SUCCESS);
}

/* Functions */
void init_hm ( int * seq, int volume )
{
  int i = 0;
  hm.solutions = 0;
  hm.MAX_length = volume;
  hm.coord = malloc(sizeof(struct coordinates_t)*volume);
  for( i = 0; i < volume; i++ )
    hm.coord[i].x = hm.coord[i].y = hm.coord[i].z = 0;
  for( i = 1; i < seq[0]; i++ )
    hm.coord[i].x = i;
  for( i = seq[0]; i < seq[0] + seq[1] - 1; i++ )
  {
    hm.coord[i].x = hm.coord[i-1].x;
    hm.coord[i].y = i - seq[0] + 1;
  }

  hm.length = seq[0] + seq[1] - 2;
  hm.last_element = 1;
  hm.last_direction = 'Y';

  hm.max.x = seq[0] - 1;
  hm.max.y = seq[1] - 1;
  hm.max.z = 0;

  hm.min.x = hm.min.y = hm.min.z = 0;
  
}

void destroy_hm()
{
  free( hm.coord );
}

void createNode(int dir )
{
  int elementLength = Sequence[ hm.last_element + 1] - 1;
  if ( globalArgs.verbose )
  {
    fprintf(stderr, ">>> Entering node %c...\n", dir);
    fprintf(stderr, "    Trying to walk element #%d (length=%d) in %c\n", hm.last_element+1, elementLength, dir);
    fprintf(stderr, "    From = %d\n", hm.length + 1);
    fprintf(stderr, "    To   = %d\n", hm.length + elementLength);
  }

  /* Check that the Hamiltonian walk wouldn't be outside bounds */
  if ( globalArgs.boundingBox )
  {
    if ( globalArgs.verbose )
      fprintf(stderr, "      Check bounding box ");
    switch (dir)
    {
    case 'X':
      if( ( hm.coord[hm.length].x + elementLength ) - hm.min.x > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    case 'x':
      if( hm.max.x - ( hm.coord[hm.length].x - elementLength ) > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    case 'Y':
      if( ( hm.coord[hm.length].y + elementLength ) - hm.min.y > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    case 'y':
      if( hm.max.y - ( hm.coord[hm.length].y - elementLength ) > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    case 'Z':
      if( ( hm.coord[hm.length].z + elementLength ) - hm.min.z > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    case 'z':
      if( hm.max.z - ( hm.coord[hm.length].z - elementLength ) > globalArgs.boundingBox - 1 )
      {
        if ( globalArgs.verbose ) fprintf(stderr, " [ FAIL ]\n");
        return;
      }
      break;
    default:
      if ( globalArgs.verbose ) fprintf(stderr, "Direction '%d' unknown\n", dir);
      exit(EXIT_FAILURE);
    }
    if ( globalArgs.verbose ) fprintf(stderr, " [ SUCCESS ]\n");
  }

  /* Check that the new node's coordinates wouldn't overlap */
  if ( globalArgs.verbose ) fprintf(stderr, "      Check overlap ");
  int candidates = 0;
  int seg = 0;
  int i = 0;
  for( seg = 0; seg < hm.last_element - 1; seg++ )
    candidates += Sequence[seg] - 1;
  while ( candidates-- )
    for ( i = 1; i <= elementLength; i++ )
    {
      switch (dir)
      {
      case 'X':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x+i && hm.coord[candidates].y == hm.coord[hm.length].y && hm.coord[candidates].z == hm.coord[hm.length].z )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      case 'x':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x-i && hm.coord[candidates].y == hm.coord[hm.length].y && hm.coord[candidates].z == hm.coord[hm.length].z )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      case 'Y':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x && hm.coord[candidates].y == hm.coord[hm.length].y+i && hm.coord[candidates].z == hm.coord[hm.length].z )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      case 'y':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x && hm.coord[candidates].y == hm.coord[hm.length].y-i && hm.coord[candidates].z == hm.coord[hm.length].z )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      case 'Z':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x && hm.coord[candidates].y == hm.coord[hm.length].y && hm.coord[candidates].z == hm.coord[hm.length].z+i )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      case 'z':
        if ( hm.coord[candidates].x == hm.coord[hm.length].x && hm.coord[candidates].y == hm.coord[hm.length].y && hm.coord[candidates].z == hm.coord[hm.length].z-i )
        {
          if ( globalArgs.verbose ) fprintf(stderr, "[ FAIL ]\n");
          return;
        }
        break;
      default:
        fprintf(stderr, "Direction '%c' not known\n", dir);
        exit(EXIT_FAILURE);
      }
    }

  if ( globalArgs.verbose ) fprintf(stderr, " [ SUCCESS ]\n");

  /* Update the hm structure with the new coordinates */
  if ( globalArgs.verbose ) fprintf(stderr, "     -----> UPDATE HM\n");

  int from = hm.length + 1;
  int to =   hm.length + elementLength;
  int prv_max[] ={hm.max.x, hm.max.y, hm.max.z};
  int prv_min[] ={hm.min.x, hm.min.y, hm.min.z};
  hm.length += elementLength;
  for ( i = from; i <= to; i++ )
  {
    hm.coord[i].x = hm.coord[i-1].x;
    hm.coord[i].y = hm.coord[i-1].y;
    hm.coord[i].z = hm.coord[i-1].z;
  }

  switch (dir)
  {
  case 'X':
    for ( i = from; i <= to; i++ )
      hm.coord[i].x = hm.coord[i-1].x + 1;
    hm.max.x < hm.coord[hm.length].x ? hm.max.x = hm.coord[hm.length].x : 0;
    break;
  case 'x':
    for ( i = from; i <= to; i++ )
      hm.coord[i].x = hm.coord[i-1].x - 1;
    hm.min.x > hm.coord[hm.length].x ? hm.min.x = hm.coord[hm.length].x : 0;
    break;
  case 'Y':
    for ( i = from; i <= to; i++ )
      hm.coord[i].y = hm.coord[i-1].y + 1;
    hm.max.y < hm.coord[hm.length].y ? hm.max.y = hm.coord[hm.length].y : 0;
    break;
  case 'y':
    for ( i = from; i <= to; i++ )
      hm.coord[i].y = hm.coord[i-1].y - 1;
    hm.min.y > hm.coord[hm.length].y ? hm.min.y = hm.coord[hm.length].y : 0;
    break;
  case 'Z':
    for ( i = from; i <= to; i++ )
      hm.coord[i].z = hm.coord[i-1].z + 1;
    hm.max.z < hm.coord[hm.length].z ? hm.max.z = hm.coord[hm.length].z : 0;
    break;
  case 'z':
    for ( i = from; i <= to; i++ )
      hm.coord[i].z = hm.coord[i-1].z - 1;
    hm.min.z > hm.coord[hm.length].z ? hm.min.z = hm.coord[hm.length].z : 0;
    break;
  default:
    fprintf(stderr, "Direction '%d' unknown\n", dir);
    exit(EXIT_FAILURE);
  }

  int prv_dir = hm.last_direction;
  hm.last_direction = dir;
  hm.last_element++;

  if ( globalArgs.energyFileName )
  {
    fprintf(globalArgs.energyFile, "%d,%d\n", countContacts(), hm.length);
  }

  /* Check if the end of the sequence has been reached */
  if (hm.last_element == sizeof (Sequence) / sizeof (int) - 1)
  {
    if ( globalArgs.verbose )
    {
      fprintf(stderr, "------------ Solution found ------------\n");
      print_hm();
      fprintf(stderr, "----------------------------------------\n");
    }
    hm.solutions++;

    if ( globalArgs.output )
    {
      if ( globalArgs.outFileName )
      {
        char fname[256] = "";
        strcat(fname, globalArgs.outFileName);
        strcat(fname, ".");
        char strnumber[5];
        sprintf(strnumber, "%04d", hm.solutions);
        strcat(fname, strnumber);

        globalArgs.outFile = fopen(fname, "w");
        if ( ! globalArgs.outFile )
        {
          fprintf(stderr, "Could not open %s for writing\n", fname);
          exit(EXIT_FAILURE);
        }
      }
      
      printCSV(Sequence, globalArgs.outFile);
      
      if ( globalArgs.outFileName )
        fclose(globalArgs.outFile);
      
    }
    
    hm.length -= elementLength;
    hm.last_direction = prv_dir;
    hm.last_element--;
    hm.max.x = prv_max[0];
    hm.max.y = prv_max[1];
    hm.max.z = prv_max[2];
    hm.min.x = prv_min[0];
    hm.min.y = prv_min[1];
    hm.min.z = prv_min[2];

    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "S" );
    
    return;
  }

  if ( globalArgs.maxSolutions && globalArgs.maxSolutions == hm.solutions )
    return;

  /* The children pointers are created depending on the direction of this
   * present node
   *   (x) -> (y/z)
   *   (y) -> (x/z)
   *   (z) -> (x/y)
   */

  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, "(" );

  switch(dir)
  {
  case 'X':
  case 'x':
    createNode('Y');
    if ( globalArgs.newickFile ){
      fprintf( globalArgs.newickFile, "+y:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    }
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('y');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "-y:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('Z');
    if ( globalArgs.newickFile )
    {
      fprintf( globalArgs.newickFile, "+z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
      if ( walked_in_z() || globalArgs.specular )
        fprintf( globalArgs.newickFile, "," );
    }
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    if ( walked_in_z() || globalArgs.specular )
    {
      createNode('z');
      if ( globalArgs.newickFile )
        fprintf( globalArgs.newickFile, "-z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    }
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    break;
  case 'Y':
  case 'y':
    createNode('X');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "+x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('x');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "-x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('Z');
    if ( globalArgs.newickFile )
    {
      fprintf( globalArgs.newickFile, "+z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
      if ( walked_in_z() || globalArgs.specular )
        fprintf( globalArgs.newickFile, "," );
    }
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    if ( walked_in_z() || globalArgs.specular )
    {
      createNode('z');
      if ( globalArgs.newickFile )
        fprintf( globalArgs.newickFile, "-z:%.3f", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    }
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    break;
  case 'Z':
  case 'z':
    createNode('X');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "+x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('x');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "-x:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('Y');
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "+y:%.3f,", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    createNode('y');
    if ( globalArgs.verbose ) fprintf(stderr, "<<<< Fallen through a node\n");
    if ( globalArgs.newickFile )
      fprintf( globalArgs.newickFile, "-y:%.3f", (float) Sequence[ (hm.last_element)+1 ] / hm.MAX_length );
    break;
  default:
    fprintf(stderr, "Direction '%d' unknown\n", dir);
    exit(EXIT_FAILURE);
  }

  if ( globalArgs.newickFile )
    fprintf( globalArgs.newickFile, ")" );

  hm.length -= elementLength;
  hm.last_direction = prv_dir;
  hm.last_element--;
  hm.max.x = prv_max[0];
  hm.max.y = prv_max[1];
  hm.max.z = prv_max[2];
  hm.min.x = prv_min[0];
  hm.min.y = prv_min[1];
  hm.min.z = prv_min[2];

  return;
}

void printCSV (int seq[], FILE * fh)
{
  int i,c;

  int segment_nb = 0;
  int segment_size = seq[0]-1;
  fprintf(fh, "%.3f,%.3f,%.3f\n",(float) hm.coord[0].x, (float) hm.coord[0].y, (float) hm.coord[0].z);
  for (i=1; i<hm.length; i++)
  {
     segment_size--;
     if ( segment_size == 0 ) {
        fprintf(fh, "%.3f,%.3f,%.3f\n",(float) hm.coord[i].x, (float) hm.coord[i].y, (float) hm.coord[i].z);
        segment_size = seq[++segment_nb]-1;
     }
  }
}

int countContacts()
{
  int i, j;
  int contacts = 0;
  for ( i = 0; i < hm.length; i++ )
  {
    for ( j = i+3; j < hm.length; j++ )
    {
      if ( abs(hm.coord[i].x - hm.coord[j].x) + abs(hm.coord[i].y - hm.coord[j].y) +abs(hm.coord[i].x - hm.coord[j].x) == 1 )
        contacts++;
    }
  }

  return contacts;
}


int is_perfect_cube(int n)
{
  int root = round(cbrt(n));
  return  n == root * root * root;
}

int walked_in_z()
{
  return abs( hm.max.z - hm.min.z );
}

void usage(char* pname)
{
    fprintf(stderr, "%s [-scb] [-M int] [-o file] [-E file] [-n int]\n", pname);
    fprintf(stderr, "  -s       Compute also specular solutions (enantiomers)\n"
                    "  -c       Do not print any outputs, only count number of\n"
                    "           solutions. Overrides -o option\n"
                    "  -b int   Extend the size of the bounding box. The Hamiltonian\n"
                    "           walk will never be larger than <int> in any direction.\n"
                    "           If the flag is not provided, the bounding box will be\n"
                    "           the smallest possible\n" 
                    "  -M int   Limits the number of solutions to find. The search\n"
                    "           will stop when the number of solutions provided is\n"
                    "           reached. A value of 0 means no limit (default)\n"
                    "  -o file  Prints the coordinates of the solutions to files\n"
                    "           <file>.0001, <file>.0002, ... If no -o flag is\n"
                    "           provided, solutions are printed to stdout by default\n"
                    "  -E file  Calculate number of contacts throughout folding.\n"
                    "           Argument to this flag is the file where the energies\n"
                    "           are printed\n"
                    "  -n file  Print the Newick format string for the search tree.\n"
                    "           Argument to this flag is the file output for the tree\n"
                    "  -v       Prints heaps of useless stuff. Mainly for debugging\n"
                    "  -h       Prints (this) help message\n");
}

void print_hm ()
{
  fprintf(stderr, "%d %d %d\n", hm.coord[0].x, hm.coord[0].y, hm.coord[0].z);
  int tot_E = sizeof (Sequence) / sizeof (int);
  int E;
  int i = 1;
  for( E = 0; E < tot_E; E++ )
  {
    int E_len = Sequence[E] - 1;
    int j;
    for( j = 0; j < E_len; j++ )
    {
      fprintf(stderr, "%d %d %d", hm.coord[i].x, hm.coord[i].y, hm.coord[i].z);
      if( j == E_len-1 )
        fprintf(stderr, " * ", hm.coord[i].x, hm.coord[i].y, hm.coord[i].z);
      else
        fprintf(stderr, "   ");
      if( i == hm.length )
        fprintf(stderr, " <--\n");
      else
        fprintf(stderr, "\n");
      i++;
    }
  }
  fprintf(stderr, "last_direction = %c\n", hm.last_direction);
  fprintf(stderr, "last_element = %d\n", hm.last_element);
  fprintf(stderr, "length = %d\n", hm.length);
  fprintf(stderr, "min.x = %d\n", hm.min.x);
  fprintf(stderr, "min.y = %d\n", hm.min.y);
  fprintf(stderr, "min.z = %d\n", hm.min.z);
  fprintf(stderr, "max.x = %d\n", hm.max.x);
  fprintf(stderr, "max.y = %d\n", hm.max.y);
  fprintf(stderr, "max.z = %d\n", hm.max.z);
  fprintf(stderr, "solutions = %d\n", hm.solutions);
}

