#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define CUBE_SIDE 3

/* Example sequences (CHANGE THE #define CUBE_SIDE ACCORDINGLY)
   1) Real Pascu cube:
   int Sequence[] = {3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2};
   2) Shorter 4x4x4 sequence
   int Sequence[] = {4,4,4,3,3,2,2,2,3,2,4,4,4,2,3,2,2,2,3,3,4,4,4,2,4,4,4,3,3,2,2};
   3) 3x3x3 cube
   int Sequence[] = {3,3,3,2,2,2,2,2,3,3,3,2,3,3,3,2,2};
*/

int Sequence[] = {3,3,3,2,2,2,2,2,3,3,3,2,3,3,3,2,2};
/* Global variables */
//int Sequence[] = {3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2};
int step,dots,total;
long int num;

int HamWalk[CUBE_SIDE*CUBE_SIDE*CUBE_SIDE][3];
int ElementNum = sizeof (Sequence) / sizeof (int);

/* Headers */
/** Prints the Hamiltonian walk to a coordinates file */
void printCSV (int HamWalk[][3], int seq[], int filenumber);
/** Calculates the contact energy of the Hamiltonian walk */
double calcEnergy (int HamWalk[][3], int dots);
/** Creates a node with coordinates and checks for size/overlap */
void CreateNode(int x, int y, int z, int dir);

/* Main */
int main ()
{
   int b = 0;
   int temp = 0;
   for (b = 0; b < ElementNum; b++){
       temp = temp + (Sequence[b]-1);
   }
   temp = temp++;
   printf("# There are %d grid positions to occupy by the given sequence\n",temp);
   //  system("read -p Continue...");
   step = 0;
   dots = 0;
   total = 0;
   num = 0;
   clock_t start = clock();

   // Create root node and enter recursive tree building
   CreateNode(0,0,0,6);

   printf ("# Total number of solutions found: %d\n",total);
   printf("# Time elapsed: %.4f seconds\n", ((double)clock() - start) / CLOCKS_PER_SEC);
   return 0;
}

/* Functions */
void printCSV (int HamWalk[][3], int seq[], int filenumber)
{
   int i,c;
   int size = CUBE_SIDE * CUBE_SIDE * CUBE_SIDE;
   char filename[256] = "res";
   char strnumber[5];
   sprintf(strnumber, "%d", filenumber);
   for (c=0;c<4-strlen(strnumber);c++)
   {
      filename[3+c] = '0';
   }
   strcat(filename,strnumber);
   strcat(filename,".csv");
   printf("%s\n",filename);
   
   
   FILE *fp;
   if ((fp = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr,"Cannot open file to write...\n");
   } 
   else if (fp = fopen(filename, "w"))
   {
      int segment_nb = 0;
      int segment_size = seq[0]-1;
      fprintf(fp, "%d.000,%d.000,%d.0000\n",HamWalk[0][0],HamWalk[0][1],HamWalk[0][2]);
      for (i=1; i<size; i++)
      {
         segment_size--;
         if ( segment_size == 0 ) {
            fprintf(fp, "%d.000,%d.000,%d.0000\n",HamWalk[i][0],HamWalk[i][1],HamWalk[i][2]);
            segment_size = seq[++segment_nb]-1;
         }
      }		
      fclose(fp);
   }
}

double calcEnergy (int HamWalk[][3], int dots)
{
   int n,j,contacts;
   double mean_contacts;
   contacts = 0;
   for (n=0;n<dots;n++)
   {
      for (j=0;j<dots;j++)
      {
         if (HamWalk[n][0] == HamWalk[j][0])
         {
            if ((HamWalk[n][1] == HamWalk[j][1]+1 && HamWalk[n][2] == HamWalk[j][2]) || (HamWalk[n][1] == HamWalk[j][1]-1 && HamWalk[n][2] == HamWalk[j][2]))
            {
               contacts++;
            }
            else if ((HamWalk[n][2] == HamWalk[j][2]+1 && HamWalk[n][1] == HamWalk[j][1]) || (HamWalk[n][2] == HamWalk[j][2]-1 && HamWalk[n][1] == HamWalk[j][1]))
            {
               contacts++;
            }
         }
         else if (HamWalk[n][1] == HamWalk[j][1])
         {
            if ((HamWalk[n][0] == HamWalk[j][0]+1 && HamWalk[n][2] == HamWalk[j][2]) || (HamWalk[n][0] == HamWalk[j][0]-1 && HamWalk[n][2] == HamWalk[j][2]))
            {
               contacts++;
            }
         }
      }
   }
   mean_contacts = contacts;
   mean_contacts = mean_contacts / dots;
   return mean_contacts;
}

void CreateNode(int x, int y, int z, int dir)
{
   int i,n,a,h,j;

   HamWalk[dots][0] = x;
   HamWalk[dots][1] = y;
   HamWalk[dots][2] = z;

  dots++;
  step++;

  /* Checks that the Hamiltonian walk is not outside the bounds of the cube
   * ISSUE: this should be fixed so that there is a general "range" funtion
   * that will check on a given direction
   */
  if (dir == 0 || dir == 1)
  {
     for (i=0;i<dots-1;i++)
     {
        if (((HamWalk[dots-1][0] - HamWalk[i][0])*(HamWalk[dots-1][0] - HamWalk[i][0])) > (CUBE_SIDE-1)*(CUBE_SIDE-1))
        {
           return;
        }
     }
   }
   else if (dir == 2 || dir == 3)
   {
      for (i=0;i<dots-1;i++)
      {
         if (((HamWalk[dots-1][1] - HamWalk[i][1])*(HamWalk[dots-1][1] - HamWalk[i][1])) > (CUBE_SIDE-1)*(CUBE_SIDE-1))
         {
            return;
         }
      }
   }
   else if (dir == 4 || dir == 5)
   {
      for (i=0;i<dots-1;i++)
      {
         if (((HamWalk[dots-1][2] - HamWalk[i][2])*(HamWalk[dots-1][2] - HamWalk[i][2])) > (CUBE_SIDE-1)*(CUBE_SIDE-1))
         {
            return;
         }
      }
   }

   /* This checks that the new node's coordinates is not overlapping with the
    * Hamiltonian walk done so far
    */
   for (n=0;n<dots;n++)
   {
      for (j=n+1;j<dots;j++)
      {
         if (HamWalk[n][0] == HamWalk[j][0] && HamWalk[n][1] == HamWalk[j][1] && HamWalk[n][2] == HamWalk[j][2])
         {
            return;
         }
      }
   }

   /*  ###########################################################
   // THIS CALCULATES THE ENERGY AND PRINTS IT ON SCREEN
   // double E = calcEnergy (HamWalk,dots);
   // fprintf(stdout, "%.4f\n",E);
   ########################################################### */

   /* Checks if the end of the sequence has been reached */
   if (step == ElementNum+1)
   {
      total++;
      printCSV(HamWalk,Sequence,total);
      printf("# Solution %d\n",total);
      return;
   }


   //---------------------------------------------------------------------------------------------------
   // Se crean los punteros a los nodos hijos dependiendo de la dirección de la trayectoria.
   // (0|1) -> (2|3|4|5)
   // (2|3) -> (0|1|4|5)
   // (4|5) -> (0|1|2|3)
   // 6     -> (0)

   /* The children pointers are created depending on the direction of this
    * present node
    * (0|1) -> (2|3|4|5)
    *   (x) -> (y/z)
    * (2|3) -> (0|1|4|5)
    *   (y) -> (x/z)
    * (4|5) -> (0|1|2|3)
    *   (z) -> (x/y)
    */
   a = Sequence[step-1];
   a = a-1;

   /* from X */
   if (dir == 0 || dir == 1)
   {
      /* to +Y */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y+(i+1);
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x,y+a,z,2);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to -Y */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y-(i+1);
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x,y-a,z,3);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to +Z */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z+(i+1);
         dots++;
      }
      CreateNode(x,y,z+a,4);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to -Z */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z-(i+1);
         dots++;
      }
      CreateNode(x,y,z-a,5);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
   }
   /* from Y */
   else if (dir == 2 || dir == 3)
   {
      /* to +X */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x+(i+1);
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x+a,y,z,0);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
          dots--;
      }
      /* to -X */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x-(i+1);
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x-a,y,z,1);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to +Z */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z+(i+1);
         dots++;
      }
      CreateNode(x,y,z+a,4);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to -Z */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z-(i+1);
         dots++;
      }
      CreateNode(x,y,z-a,5);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
   }
   /* from Z */
   else if (dir == 4 || dir == 5)
   {
      /* to +X */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x+(i+1);
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x+a,y,z,0);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to -X */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x-(i+1);
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x-a,y,z,1);

      step--;
      dots--;

      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to +Y */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y+(i+1);
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x,y+a,z,2);
      
      step--;
      dots--;
      
      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      /* to -Y */
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x;
         HamWalk[dots][1] = y-(i+1);
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x,y-a,z,3);
      
      step--;
      dots--;
      
      for (i=0;i<a-1;i++)
      {
          dots--;
      }
   }
   /* Starting dummy direction */
   else if (dir == 6)
   {
      for (i=0;i<a-1;i++)
      {
         HamWalk[dots][0] = x+(i+1);
         HamWalk[dots][1] = y;
         HamWalk[dots][2] = z;
         dots++;
      }
      CreateNode(x+a,y,z,0);
      
      step--;
      dots--;
      
      for (i=0;i<a-1;i++)
      {
         dots--;
      }
      step--;
   }
   return;
}


