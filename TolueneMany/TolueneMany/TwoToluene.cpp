#include <stdio.h>
#include <math.h>

#define CCb  1.4
#define CCm 1.51
#define CHb  1.08
#define CHm 1.09
#define cos120 -0.5
#define sin120 0.866025
#define cos1095 -0.3338
#define sin1095 0.94264


#define originx 15.0
#define originy 15.0
#define originz 15.0

void coordinates(double, double, double, int);
void multiCoordinates(int, int, int);
void bonds(int);
void manyBonds(int);
void angles(int);
void getAngles(int);
void outputAngles(int [][5], int);
void setVelocities(int, double, double, double);
void velocity(int);
void partOne(int, int, int);
void masses();
void pairCoeffs();
void bondCoeffs();
void angleCoeffs();
double boxSize();

int main () {
	int molx = 1;
	int moly = 1;
	int molz = 1;

	//box size version 1 (originx, originy, originz);
	//double low = originx - (15-12.5);
	//double high = ceil(originx + (7.9 * molx));

	//box size version 2//
	double largestDim = boxSize();
	printf("\n%f\n", largestDim);
	double low = originx - (15-12);
	double high = ceil(originx + (largestDim * molx));

	//box size ver 3//


	partOne(molx*moly*molz, low, high);
	multiCoordinates(molx, moly, molz);
	//velocity(molx*moly*molz);
	manyBonds(molx*moly*molz);
	angles(molx*moly*molz);

}



double boxSize(){

double L = 5.15 - 0.85;
double t = 3.89 - 2.11;
double w = 6.75 - 1.00;

double delta = 14.0;
//double halfDelta = delta;

double realL = L + delta;
double realt = t + delta;
double realw = w + delta;

if( (realL > realt) && (realL > realw) ){
	return realL;
}
else if( (realt > realL) && (realt > realw) ){
	return realt;
}
else{
	return realw;
}




}

void multiCoordinates(int n1, int n2, int n3){

	
double L = 5.15 - 0.85;
double t = 3.89 - 2.11;
double w = 6.75 - 1.00;

double delta = 12.0;

double a = L + delta;
double b = t + delta;
double c = w + delta;
//these are lattice constants

//int n1 = 2;
//int n2 = 1;
//int n3 = 1;

int noAtoms = (n1*n2*n3) * 15;

	FILE *file1;
	file1 = fopen("tol3.xyz","w+"); 
	fprintf(file1, "%d\n", noAtoms);
	fprintf(file1, "\n");
	fclose(file1);
	int molecule = 1;

for(int i = 0;i<n1;i++){
  double xorig= (i*1.0)*a;
  for(int j=0;j<n2;j++){
    double yorig = (j*1.0)*b;
    for(int k=0;k<n3;k++){
      double zorig=(k*1.0)*c;
      //let's build a molecule "centered" on xorigin,yorigin,zorigi
      coordinates(originx+xorig, originy+yorig, originz+zorig, molecule);
	  molecule++;
    }
  }
}
}

//need to find xlow, ylow, zlow//

void coordinates(double startx, double starty, double startz, int molecule) {
  double x[16],y[16],z[16];
  double cos30=sqrt(3.0)/2.0;
  double sin30= 0.5;
  double sintet=sqrt(8.0)/3.0;

  int CA = 1;
  int CT = 2;
  int HA = 3;
  int HT = 4;

  int atom_type[] = {0, HA, HA, CA, CA, CA, HA, HA, CA, CA, CA, HA, CT, HT, HT, HT};

  int i;
  for(i=1;i<13;i++){y[i]=starty;}
  
  x[0] = 0; y[0] = 0; z[0] = 0;

  x[1]=startx;z[1]=startz;

  x[4]=x[1];z[4]=z[1]+CHb;

  x[3]=x[4]-CCb*cos30;  z[3]=z[4]+CCb*sin30;
  x[5]=x[4]+CCb*cos30;  z[5]=z[4]+CCb*sin30;

  x[2]=x[3]-CHb*cos30;  z[2]=z[3]-CHb*sin30;
  x[6]=x[5]+CHb*cos30;  z[6]=z[5]-CHb*sin30;

  /////////////////////////////////////////
  //   these still need to be done  //
  x[9]=x[1];z[9]=z[4]+CCb*(1+2*sin30);

  x[8]=x[9]-CCb*cos30; z[8]=z[9]-CCb*sin30;
  x[10]=x[9]+CCb*cos30;z[10]=z[9]-CCb*sin30;

  x[7]=x[8]-CHb*cos30; z[7]=z[8]+CHb*sin30;
  x[11]=x[10]+CHb*cos30; z[11]=z[10]+CHb*sin30;

  x[12]=x[9]; z[12]=z[9]+CCm;

  ///////////////////////////////

  x[12]=x[9];z[12]=z[9]+CCm;

  z[13]=z[14]=z[15]=z[12]+ CHm/3.0;//cos(109.5) = - 1/3

  x[13]=x[12]+CHm*sintet*1.0;//sin(109.5) = sintet = sqrt(8.0)/3
  x[14]=x[12]-CHm*sintet*(0.5);//cos(120o)=-1/2
  x[15]=x[12]-CHm*sintet*(0.5);//cos(240o)=-1/2

  y[13]=y[12]+0.0;
  y[14]=y[12]+CHm*sintet*sqrt(3.0)/2.0;//sin(120o)=+sqrt(3.0)/2  - factor of sintet was missing here
  y[15]=y[12]-CHm*sintet*sqrt(3.0)/2.0;//sin(240o)=-sqrt(3.0)/2  - factor of sintet was missing here

  /*
  x[13] = x[12] + CHm*(sin(30.0/180.0 * 3.142))*(cos(( (109.5*1)/180) * 3.142)) ;
  x[14] = x[12] + CHm*(sin(30.0/180.0 * 3.142))*(cos( ((109.5*2)/180) * 3.142));
  x[15] = x[12] + CHm*(sin(30.0/180.0 * 3.142))*(cos( ((109.5*3)/180) * 3.142));

  y[13] = y[12] + CHm*(sin(30.0/180.0 * 3.142))*sin(((109.5*1)/180) * 3.142);
  y[14] = y[12] + CHm*(sin(30.0/180.0 * 3.142))* sin( ((109.5*2)/180) * 3.142);
  y[15] = y[12] + CHm*(sin(30.0/180.0 * 3.142))* sin( ((109.5*3)/180) * 3.142);
  */



/////////////////////////////////////////////
char atomName[] = {'o', 'H', 'H', 'C', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'H', 'C', 'H', 'H', 'H'};


//creating xyz file//
	FILE *file1;
	file1 = fopen("tol3.xyz","a+"); 
	//fprintf(file1, "%d\n", 15);
	//fprintf(file1, "\n");

	int j;
	for(j = 1; j < 16; j++){
		fprintf(file1, "%c\t%5.5f\t%5.5f\t%5.5f\n", atomName[j], x[j], y[j], z[j]);}
	
	fclose(file1); /*done!*/

///////////////////////////////////// 

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double charge[] = {0.0, 0.1150, 0.1150, -0.115, -0.115, -0.115, 0.1150, 0.1150, -0.115, -0.115, -0.115, 0.1150, -0.065, 0.06, 0.06, 0.06};
	
/////////////////////////////	dipole calculation	/////////////////////////
  if(molecule < 10)
  {
	double px = 0.0;
	double py = 0.0;
	double pz = 0.0;
	for(int m = 1; m <16; m++){
		px = px + ((x[m])*(charge[m]));
		py = py + ((y[m])*(charge[m]));
		pz = pz + ((z[m])*(charge[m]));
	}
	double dipole = sqrt( (px*px) + (py * py) + (pz * pz) );
	printf ("\n%s %f\n", "Dipole Moment", dipole);
  }
  //////////////////////////////////////////////////////////


		FILE *file;
		
		/*if(molecule == 1){
			file = fopen("atomOutput.txt","w+"); 
		else{
			file = fopen("atomOutput.txt","a+");
		}
		*/

		file = fopen("atomOutput.txt","a+");

		if(molecule == 1){
			fprintf(file, "%s\n", "Atoms");
			fprintf(file, "\n");
		}
		
		int k;
		for(k = 1; k < 16; k++){
			fprintf(file, "%d\t%d\t%d\t%5.3f\t%5.2f\t%5.2f\t%5.2f\n", (k + ((molecule-1) * 15)), molecule, atom_type[k], charge[k], x[k], y[k], z[k]);

			}
		fclose(file); /*done!*/
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////// Finished printing output to atomOutput.txt///////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void manyBonds(int molecule){
	
	for(int i = 0; i < molecule; i++){
		bonds(i+1);
	}
}

void bonds(int molecule){

	int bond[15][4] = {
		{1, 1, 13, 12},      
		{2, 1,  14,      12},      
		{3,   1,  15,      12},
		{4,   2,  9,	  12},		
		{5,   3,  8,       9},         
		{6,   4,  7,       8},
		{7,   3,  9,       10},
		{8,   4,  10,      11},
		{9,   3,  8,	  3},
		{10,   4,  2,	  3},
		{11,   3,  3,	  4},
		{12,   4,  1,       4},
		{13,   3,  4,       5},
		{14,   4,  5,	  6},
		{15,   3,  5,       10}
	};

		FILE *file;
		file = fopen("atomOutput.txt","a+"); /* apend file (add text to a file or create a file if it does not exist.*/
		
		if(molecule == 1){
			fprintf(file, "\n%s\n", "Bonds");
			fprintf(file, "\n");
		}
		
		int k;
		for(k = 1; k < 16; k++){
			fprintf(file, "%d\t%d\t%d\t%d\n", (k + ((molecule-1) * 15)), bond[k-1][1], (bond[k-1][2] + ((molecule-1) * 15)), (bond[k-1][3] + ((molecule-1)*15)));

			}
		fclose(file);


}

void angles(int molecule){
	getAngles(molecule);
}

void getAngles(int molecule){

//	int n;
  // long elapsed_seconds;
  char line[200];
  // clrscr();
  int angles[24][5];

   FILE *fr = fopen ("angles.txt", "rt");  /* open the file for reading */
   int i = 0;

   while(fgets(line, 200, fr) != NULL)
   {
	 /* get a line, up to 80 chars from fr.  done if NULL */
	 sscanf (line, "%d %d %d %d %d", &angles[i][0], &angles[i][1], &angles[i][2], &angles[i][3], &angles[i][4]);
	 /* convert the string to a long int */
	 printf ("%d %d %d %d %d\n", angles[i][0], angles[i][1], angles[i][2], angles[i][3], angles[i][4]);
	 i++;
   }
   fclose(fr);
   outputAngles(angles, molecule);
}

void outputAngles(int arg[][5], int molecule){

		FILE *file;
		file = fopen("atomOutput.txt","a+"); /* apend file (add text to a file or create a file if it does not exist.*/
		
		

	for(int i = 1; i < (molecule+1); i++){
		if(i == 1){
			fprintf(file, "\n%s\n", "Angles");
			fprintf(file, "\n");
		}
		for(int j = 1; j < 25; j++){
			fprintf(file, "%d\t%d\t%d\t%d\t%d\n", (j + ((i-1) * 24)), arg[j-1][1], (arg[j-1][2] + ((i-1) * 15)), (arg[j-1][3] + ((i-1) * 15)), (arg[j-1][4] + ((i-1) * 15)) );
			}
	}

	fclose(file);

}

void velocity(int mol){
	
	FILE *file;
	file = fopen("atomOutput.txt","a+");
	fprintf(file, "\n%s\n", "Velocities");
	fprintf(file, "\n");
	fclose(file);

	setVelocities(1, 2.5, 0.0, 0.0);
	
	setVelocities(2, -2.5, 0.0, 0.0);

}



void setVelocities(int mol, double vx, double vy, double vz){
		
	FILE *file;
	file = fopen("atomOutput.txt","a+"); /* apend file (add text to a file or create a file if it does not exist.*/
			
	for(int i = 1; i < 16; i++){
			fprintf(file, "%d\t%5.2f\t%5.2f\t%5.2f\n", (i + ((mol-1) * 15)), vx, vy, vz);
		}

	fclose(file);

}


void partOne(int molecule, int low, int high){
		FILE *file;
		file = fopen("atomOutput.txt","w+");
		
		fprintf(file, "%s\n\n", "LAMMPS Description");
		
		fprintf(file, "\t%d  %s\n", (15*molecule), "atoms");
		fprintf(file, "\t%d  %s\n", (15*molecule), "bonds");
		fprintf(file, "\t%d  %s\n\n", (24*molecule), "angles");

		fprintf(file, "\t%d  %s\n", 4, "atom types");
		fprintf(file, "\t%d  %s\n", 4, "bond types");
		fprintf(file, "\t%d  %s\n", 5, "angle types");

		fprintf(file, "\n\n\n");
		fprintf(file, "%d %d %s %s\n", low, high, "xlo", "xhi");
		fprintf(file, "%d %d %s %s\n", low, high, "ylo", "yhi");
		fprintf(file, "%d %d %s %s\n", low, high, "zlo", "zhi");

		fprintf(file, "\n\n");
		fclose(file);

		masses();
		pairCoeffs();
		bondCoeffs();
		angleCoeffs();

}

void masses(){
		FILE *file;
		file = fopen("atomOutput.txt","a+");
		
		fprintf(file, "\n%s\n\n", "Masses");
		fprintf(file, "\t%d\t%5.2f\n", 1, 12.0);
		fprintf(file, "\t%d\t%5.2f\n", 2, 12.0);
		fprintf(file, "\t%d\t%5.2f\n", 3, 1.0);
		fprintf(file, "\t%d\t%5.2f\n", 4, 1.0);
		fprintf(file, "\n");
		fclose(file);
}

void pairCoeffs(){
		
		FILE *file;
		file = fopen("atomOutput.txt","a+");
		
		fprintf(file, "%s\n\n", "Pair Coeffs");
		fprintf(file, "\t%d\t%5.2f\t%5.3f\n", 1, 0.07, 3.550);
		fprintf(file, "\t%d\t%5.2f\t%5.3f\n", 2, 0.066, 3.50);
		fprintf(file, "\t%d\t%5.2f\t%5.3f\n", 3, 0.030, 2.420);
		fprintf(file, "\t%d\t%5.2f\t%5.3f\n\n", 4,  0.030,  2.5);
		fclose(file);
}

void bondCoeffs(){

	FILE *file;
	file = fopen("atomOutput.txt","a+");

	fprintf(file, "%s\n\n", "Bond Coeffs");
	
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 1,  340.0,    1.090 );
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 2,  317.00,   1.51 );
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 3,  469.00,   1.4 );
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n\n", 4,  367.00,   1.0800);

	fclose(file);

}

void angleCoeffs(){

	FILE *file;
	file = fopen("atomOutput.txt","a+");

	fprintf(file, "%s\n\n", "Angle Coeffs");
	
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 1,   35.00,  109.500000);
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 2,   33.00,  107.800000);
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 3,   63.00,  120.0);
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n", 4,   35.00,  120.00);
	fprintf(file, "\t%d\t%5.2f\t%5.2f\n\n", 5,   70.0,   120.0);

	fclose(file);

}
