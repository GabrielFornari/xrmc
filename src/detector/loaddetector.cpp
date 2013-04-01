/*
Copyright (C) 2013 Bruno Golosio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
///////////////////////////////////
//     loaddetector.cpp          //
//        31/01/2013             //
//   author : Bruno Golosio      //
///////////////////////////////////
// Load detector parameters, position, orientation and size
//

#include <iostream>
#include <string>
#include "xrmc.h"
#include "xrmc_gettoken.h"
#include "xrmc_exception.h"
#include "xrmc_math.h"
#include "xrmc_loaddetector.h"

using namespace std;
using namespace gettoken;

//////////////////////////////////////////////////////////////////////
int detectorarray::Load(istream &fs)
  // Loads detector array position/orientation/size
{
  int i;
  vect3 x0, u;
  matr3 R;
  double theta;
  string comm="";

  cout << "Detector Array parameters, position, orientation and size file\n";

 // get a command/variable name from input file
  while (GetToken(fs, comm)) {
    // parse the command and decide what to do
    //
    // check if it's a command for setting an input device name
    if (ParseInputDeviceCommand(fs, comm)) continue;
    else if(comm=="NPixels") { // set the number of pixels (rows and columns)
      GetIntToken(fs, &NX);
      GetIntToken(fs, &NY);
      cout << "Pixel number (NX x NY): " << NX << " x " << NY << "\n";
    }
    else if(comm=="PixelSize") { // set the pixel size in x and y directions
      GetDoubleToken(fs, &PixelSizeX);
      GetDoubleToken(fs, &PixelSizeY);
      cout << "Pixel size (Sx x Sy) (cm): " << PixelSizeX << " x "
	   <<  PixelSizeY << "\n";
    }
    else if(comm=="Shape") { // detector elements shape
                             // (rectangular or elliptical)
      GetIntToken(fs, &Shape);
      cout << "Detector elements shape (0 rectangular/1 elliptical): "
	   << Shape << "\n";
    }
    else if(comm=="dOmegaLim") { // set the cut on solid angle
      GetDoubleToken(fs, &dOmegaLim);
      if (dOmegaLim==0) dOmegaLim=2*PI; // if 0 set it to the default value
      cout << "Cut on solid angle from interaction point to a single pixel: "
	   << dOmegaLim << "\n";
    }
    else if(comm=="X") { // set the detector center coordinates
      cout << "Detector array center position :\t"; 
      for (i=0; i<3; i++) {
	GetDoubleToken(fs, &X.Elem[i]);
	//cout << X[i] << "\t";
      }
      cout << X << endl;
      //cout << "\n";
    }
    else if(comm=="uk") { // direction of the normal to the detector surface uk
      cout << "Detector orientation :\n"; 
      cout << "\tuk vector:\t"; 
      for (i=0; i<3; i++) {
	GetDoubleToken(fs, &uk.Elem[i]);
	//cout << uk[i] << "\t";
      }
      cout << uk << endl;
      //cout << "\n";
    }	
    else if(comm=="ui") { // direction of ui, parallel to the detector rows
      cout << "Detector orientation :\n"; 
      cout << "\tui vector:\t"; 
      for (i=0; i<3; i++) {
	GetDoubleToken(fs, &ui.Elem[i]);
	//cout << ui[i] << "\t";
      }
      cout << ui << endl;
      //cout << "\n";
    }	
    else if(comm=="ExpTime") { // set the Exposure Time
      GetDoubleToken(fs, &ExpTime);
      cout << "Exposure Time (s): " << ExpTime << "\n";
    }	
    else if(comm=="PhotonNum") { // Multiplicity of simulated events per pixel
      GetIntToken(fs, &PhotonNum);
      cout << "Multiplicity of simulated events per pixel: "
	   << PhotonNum << "\n";
    }
    else if(comm=="RandomPixelFlag") { // enable/disable random point on pixels
      GetIntToken(fs, &RandomPixelFlag);
      cout << "Enable random point on pixels (0/1): " << RandomPixelFlag
	   << "\n";
    }
    else if(comm=="PoissonFlag") { // flag to enable/disable Poisson statistic
      GetIntToken(fs, &PoissonFlag);
      cout << "Enable Poisson statistic on pixel counts (0/1): "
	   << PoissonFlag << "\n";
    }
    else if(comm=="RoundFlag") { // enable/disable round counts to integer
      GetIntToken(fs, &RoundFlag);
      cout << "Round pixel counts to integer (0/1): " << RoundFlag << "\n";
    }
    else if(comm=="HeaderFlag") { // flag to use or not header in output file
      GetIntToken(fs, &HeaderFlag);
      cout << "Use header in output file (0/1): " << HeaderFlag << "\n"; 
    }
    else if(comm=="RunningFasterFlag") { //columns(0) or rows(1) running faster
      GetIntToken(fs, &RunningFasterFlag);
      if (RunningFasterFlag==0) 
	cout << "Columns running faster than rows in output file\n"; 
      else
	cout << "Rows running faster than columns in output file\n"; 
    }
    else if(comm=="PixelType") { // set the pixel content type
      GetIntToken(fs, &PixelType);
      cout << "Pixel content type: " << PixelType << "\n"; 
      cout << "0: fluence, 1: energy fluence, 2: fluence(E), " 
	"3: energy fluence(E)\n";
    }
    else if(comm=="Emin") { // set the minimum bin energy
      GetDoubleToken(fs, &Emin);
      cout << "\tEmin: " << Emin << "\n"; 
    }	
    else if(comm=="Emax") { // set the maximum bin energy
      GetDoubleToken(fs, &Emax);
      cout << "\tEmax: " << Emax << "\n"; 
    }	
    else if(comm=="NBins") { // set the number of energy bins
      GetIntToken(fs, &NBins);
      cout << "\tNBins: " << NBins << "\n"; 
    }	
    else if(comm=="SaturateEmin") { // flag to saturate energies < Emin
      GetIntToken(fs, &SaturateEmin);
      cout << "\tSaturate energies lower than Emin (0/1):"
	     << SaturateEmin << "\n"; 
    }
    else if(comm=="SaturateEmax") { // flag to saturate energies > Emax
      GetIntToken(fs, &SaturateEmax);
      cout << "\tSaturate energies greater than Emax (0/1):"
	   << SaturateEmax << "\n";
    }
    else if(comm=="Rotate") { // detector rotation
      cout << "Detector rotation :\n"; 
      cout << "\tPoint on rotation axis x0:\t";
      for (int i=0; i<3; i++) {
	GetDoubleToken(fs, &x0.Elem[i]); // translation components
      }
      cout << x0 << endl;
      cout << "\tRotation axis direction u:\t"; 
      for (int i=0; i<3; i++) {
	GetDoubleToken(fs, &u.Elem[i]); // get rotation axis direction
      }
      u.Normalize(); // normalize axis direction
      cout << u << endl;
      cout << "\tRotation angle theta (degrees): ";
      GetDoubleToken(fs, &theta); // get angle in degrees
      cout << theta << endl;
      R = matr3::RotMatr(u, -theta); // build rotation matrix
      X -= x0; // translate detector position: rotation axis-> origin
      X = R*X; // rotate detector position
      uk = R*uk; // rotate detector uk direction
      ui = R*ui; // rotate detector ui direction
      X += x0; // translate back detector position
    }
    else if(comm=="End") {
      break;
    }
    else if(comm=="") {
      cout << "Empy string\n";
    }
    else {
      throw xrmc_exception("syntax error in detectorarray input file"); 
    }
  }

  return 0;
}

int detectorarray::Save(string file_name)
  // Save detector array contents
{
  FILE *fp;

  cout << "Output File: " << file_name << "\n";
  if ((fp = fopen(file_name.c_str(),"wb")) == NULL)
    throw xrmc_exception("Output file cannot be open.\n");

  if (HeaderFlag==1) {
    fwrite(&ModeNum, sizeof(int), 1, fp);
    fwrite(&NX, sizeof(int), 1, fp);
    fwrite(&NY, sizeof(int), 1, fp);
    fwrite(&PixelSizeX, sizeof(double), 1, fp);
    fwrite(&PixelSizeY, sizeof(double), 1, fp);
    fwrite(&ExpTime, sizeof(double), 1, fp);
    fwrite(&PixelType, sizeof(int), 1, fp);
    fwrite(&NBins, sizeof(int), 1, fp);
    fwrite(&Emin, sizeof(double), 1, fp);
    fwrite(&Emax, sizeof(double), 1, fp);
  }

  fwrite(Image[0][0], sizeof(double), ModeNum*N*NBins, fp);
    
  fclose(fp);

  return 0;
}

//////////////////////////////////////////////////////////////////////
int detectorarray::SetDefault()
// set default values for detectorarray parameters
{
  InputDeviceName[0] = "Sample"; // input device name
  NX = NY = 1; // number of rows and columns

  PixelSizeX = PixelSizeY = 1; // Pixel size (Sx x Sy) = 1x1 cm2
  Shape = 0; // rectangular pixel shape
  dOmegaLim=2*PI; //Cut on solid angle from interaction point to a single pixel
  X.Set(0, 50, 0); // Detector center position 
  // Detector array orientation :
  uk.Set(0,-1,0); // uk has direction opposite to y axis
  ui.Set(1,0,0);// ui has the same direction as the x axis
  OrthoNormal(ui, uj, uk); // evaluates uj to form a orthonormal basis

  ExpTime = 1; // 1 sec. Exposure Time
  PhotonNum = 1; // Num. of simulated photons per detector pixel
  RandomPixelFlag = 1; // Enable random point on pixels 
  PoissonFlag = 0; // Poisson statistic on pixel counts disabled
  RoundFlag = 0;   // Round pixel counts round to integer disabled
  HeaderFlag = 0; // header in output file disabled
  RunningFasterFlag = 0; // columns running faster than rows in output file
  PixelType = 0; // Pixel content type = 0: fluence
  NBins = 1; // 1 energy bin
  Emin = 0; // minimum bin energy is 0 keV
  Emax = 100; // maximum bin energy is 100 keV
  SaturateEmin = 0; // do not Saturate energies lower than Emin
  SaturateEmax = 0; // do not Saturate energies greater than Emax

  return 0;
}