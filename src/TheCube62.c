#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define CUBE_SIDE 4

/* EXAMPLE SEQUENCES FOR THE CUBES (CHANGE THE #define CUBE_SIDE ACCORDINGLY)
   1) Real Pascu cube:
   int Sequence[] = {3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2};
   2) Shorter 4x4x4 sequence
   int Sequence[] = {4,4,4,3,3,2,2,2,3,2,4,4,4,2,3,2,2,2,3,3,4,4,4,2,4,4,4,3,3,2,2};
   3) 3x3x3 cube
   int Sequence[] = {3,3,3,2,2,2,2,2,3,3,3,2,3,3,3,2,2};
*/
int Sequence[] = {3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2};

int step,dots,total;
long int num;
FILE *trajFile;

int HamWalk[CUBE_SIDE*CUBE_SIDE*CUBE_SIDE][3];
int ElementNum = sizeof (Sequence) / sizeof (int);

void CreateNode(int x, int y, int z, int dir);
double calcEnergy (int HamWalk[][3], int dots);

void PrintPdb (int dots, int HamWalk[][3], int filenumber)
{
   int i,c;
   char filename[8] = "res";
   char strnumber[5];
   sprintf(strnumber, "%d", filenumber);
   for (c=0;c<4-strlen(strnumber);c++)
   {
      filename[3+c] = '0';
   }
   strcat(filename,strnumber);
   
   
   FILE *fp;
   if ((fp = fopen(filename, "w")) == NULL)
   {
      printf("Cannot open file to write...\n");
   } 
   else if (fp = fopen(filename, "w"))
   {
      for (i=0;i<dots;i++)
      {
         fprintf(fp, "ATOM     %2d  BOX BOX     1      %2d.000  %2d.000  %2d.0000\n",i+1,HamWalk[i][0],HamWalk[i][1],HamWalk[i][2]);
      }		
      for (i=0;i<dots-1;i++)
      {
         fprintf(fp, "CONECT   %2d   %2d           \n",i+1,i+2);
      }
      fclose(fp);
   }
}

void printCSV (int HamWalk[][3], int seq[], int filenumber)
{
   int i,c;
   int size = CUBE_SIDE * CUBE_SIDE * CUBE_SIDE;
   char filename[12] = "res";
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

void printXYZ (int HamWalk[][3], int seq[], int length, FILE* fp)
{
   int i = length-1;
   int segment_nb = 0;
   int tot_segments = 0;
   while ( i ) {
      tot_segments++;
      i = i - (seq[segment_nb++]-1);
   }
   fprintf(fp, "%d\n",tot_segments+1);
   fprintf(fp, "E=%.4f\n",calcEnergy(HamWalk,length));
   char buff[100];
   sprintf(buff, "   C %10.5f %10.5f %10.5f\n",(double)HamWalk[0][0],(double)HamWalk[0][1],(double)HamWalk[0][2]);
   fprintf(fp, "%s", buff);

   segment_nb = 0;
   for (segment_nb = 0; segment_nb < tot_segments; segment_nb++)
   {
      i += seq[segment_nb] - 1;
      sprintf(buff, "   C %10.5f %10.5f %10.5f\n",(double)HamWalk[i][0],(double)HamWalk[i][1],(double)HamWalk[i][2]);
      fprintf(fp, "%s", buff);
   }		
}

void printPDBtraj (int HamWalk[][3], int seq[], int length, FILE* fp, long int num)
{
   int i = length-1;
   int segment_nb = 0;
   int tot_segments = 0;
   while ( i ) {
      tot_segments++;
      i = i - (seq[segment_nb++]-1);
   }
   fprintf(fp, "MODEL %8li\n",num+1);
   fprintf(fp, "ATOM     %2d  BOX BOX     1      %2d.000  %2d.000  %2d.0000\n",1,HamWalk[0][0],HamWalk[0][1],HamWalk[0][2]);
   for (segment_nb = 0; segment_nb < tot_segments; segment_nb++)
   {
      i += seq[segment_nb] - 1;
      fprintf(fp, "ATOM     %2d  BOX BOX     1      %2d.000  %2d.000  %2d.0000\n",segment_nb+2,HamWalk[i][0],HamWalk[i][1],HamWalk[i][2]);
   }		
   for (segment_nb = 0; segment_nb < tot_segments; segment_nb++)
   {
      fprintf(fp, "CONECT   %2d   %2d           \n",segment_nb+1,segment_nb+2);
   }
   fprintf(fp, "END\nENDMDL\n");
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

// -----------------------------------------------------------------------------------
int main ()
{
   int b = 0;
   int temp = 0;
   for (b=0;b<ElementNum;b++){temp = temp + (Sequence[b]-1);}
   temp = temp++;
   printf("# There are %d grid positions to occupy by the given sequence\n",temp);
   //  system("read -p Continue...");
   step = 0;
   dots = 0;
   total = 0;
   num = 0;
   clock_t start = clock();

   char filename[] = "full_trajectory.pdb";
   if ((trajFile = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr,"Cannot open file to write...\n");
   } 
   // Create root node and enter recursive tree building
   CreateNode(0,0,0,6);

   fclose(trajFile);

   printf ("# Total number of solutions found: %d\n",total);
   printf("# Time elapsed: %.4f seconds\n", ((double)clock() - start) / CLOCKS_PER_SEC);
   return 0;
}
// -----------------------------------------------------------------------------------


void CreateNode(int x, int y, int z, int dir)
{
   int i,n,a,h,j;

   HamWalk[dots][0] = x;
   HamWalk[dots][1] = y;
   HamWalk[dots][2] = z;

  dots++;
  step++;

  // Cube size condition
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

   // No overlap condition
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
   /* Print trajectory on file */
   //if ( step > 1 ) printXYZ(HamWalk,Sequence,dots,trajFile);
   if ( step > 1 ) printPDBtraj(HamWalk,Sequence,dots,trajFile,num++);

   // End of sequence??
   if (step == ElementNum+1)
   {
      total++;
      //PrintPdb(dots,HamWalk,total);
      //printCSV(HamWalk,Sequence,total);
      printf("# Solution %d\n",total);
      return;
   }


   //---------------------------------------------------------------------------------------------------
   // Se crean los punteros a los nodos hijos dependiendo de la dirección de la trayectoria.
   // (0|1) -> (2|3|4|5)
   // (2|3) -> (0|1|4|5)
   // (4|5) -> (0|1|2|3)
   // 6     -> (0)

   a = Sequence[step-1];
   a = a-1;
   //---------------------------------------------------------------------------------------------------
   if (dir == 0 || dir == 1)
   {
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
   //------------------------------------------------------------------------------------------------
   else if (dir == 2 || dir == 3)
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
   //-----------------------------------------------------------------------------------------------------
   else if (dir == 4 || dir == 5)
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
      //---------------------------------------------------------------------------------------------------
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
   //-------------------------------------------------------------------------------------------------------
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
   else
   {
      printf("FATAL ERROR: direction does not exist\n");
      system("read -p BROKEN OPERATION...");
      return;
   }
   return;
}
//---------------------------------------------------------------------------------------------------------------




