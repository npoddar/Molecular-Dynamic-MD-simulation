#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define xcen 168.69625
#define ycen 143.12145
#define zcen 259.21892

float myRadius[540];

void radiusTip(char *);
void printRadius();
void Ravg();

int main(){

	char *NPFile;
	NPFile = (char*) malloc(2000000);

	printf("Enter the file that contains the nanoparticle coordinates : ");
	scanf(" %s", NPFile);

	radiusTip(NPFile);	
	printRadius();
	Ravg();

}

void radiusTip(char *NPdumpFile){

	//this method puts tip radius for each of the 540 molecules into float tipradius[540]//

	FILE *input = fopen(NPdumpFile, "rt");
	

	char line[2000000];
	int atomperMolecule = 0;
	int i = 0;
	float tipradiustemp = 0.0;
	float wby2 = 0.0;
	float hby2 = 0.0;

	while( (fgets(line, 2000000, input) != NULL) ){
		int atomid, molid;
		float xcoor, ycoor, zcoor;

		//printf("Entered the while loop\n");

		int scan = sscanf(line, "%d %d %f %f %f", &atomid, &molid, &xcoor, &ycoor, &zcoor);
		
		if(scan == 5) {
			//printf("Entered scan = 5 case\n");
			float deltax = (xcen - xcoor);
			deltax = deltax*deltax;

			float deltay = (ycen - ycoor);
			deltay = deltay*deltay;

			float whalftemp = sqrt(deltax + deltay);

			if(whalftemp > wby2){
				wby2 = whalftemp;
			}

			float deltaz = (zcen - zcoor);
			
			if(deltaz > hby2){
				hby2 = deltaz;
			}

			deltaz = deltaz*deltaz;

			float radius = sqrt(deltax + deltay + deltaz);			

			atomperMolecule++;

			if(radius > tipradiustemp){
				tipradiustemp = radius;
			}

			if (atomperMolecule == 38)
			{
				//printf("Entered atomPerMolecule statement\n");
				//printf("Value of radius = %f\n", tipradiustemp);

				myRadius[i] = tipradiustemp;

				//printf("Value of radius in 

				atomperMolecule = 0;
				//molid++;
				i++;
				tipradiustemp = 0.0;
				//printf("End of atompermolecule block= 38\n");
			}

			//printf("end of if statement Atompermolecule = 38\n");

		}//if scan = 5//

		//printf("End of if statement scan = 5\n");

	}//end of while loop

	if(i != 540){
		printf("No of molecules = %d\n", i);
		while(true){
			;
		}
	}

	printf("W/2 = %f\n", wby2);
	printf("H/2 = %f\n", hby2);

	fclose(input);

}//end of findTip

void printRadius(){

	FILE *output = fopen("TipRadius.all", "w+");

	for(int i = 0; i < 540; i++){
		fprintf(output, "Radius of mol %d = %f\n", i, myRadius[i]);
	}

	fclose(output);

}

void Ravg(){

	float ravg = 0.0;

	float rsqavg = 0.0;

	for(int i = 0; i < 540; i++){
		ravg = ravg + myRadius[i];

		rsqavg = rsqavg + (myRadius[i] * myRadius[i]);
	}

	ravg = ravg / 540;
		
	rsqavg = rsqavg / 540;

	printf("Ravg = %f\n", ravg);
	printf("Rsqavg = %f\n", rsqavg);

	float deltaR_sq = rsqavg - ravg*ravg;

	printf("<(delta R)^2> = %f\n", deltaR_sq);

}


