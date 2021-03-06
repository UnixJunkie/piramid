/*******************************************************************************
solutionInfo.cpp - Piramid
 
Copyright (C) 2005-2010 by Silicos NV
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/



#include <solutionInfo.h>



SolutionInfo::SolutionInfo(void)
: refName("")
, refAtomVolume(0.0)
, refCenter(0,0,0)
, refRotation(3,3,0)
, dbName("")
, dbAtomVolume(0.0)
, dbMol()
, dbCenter(0,0,0)
, dbRotation(3,3,0)
, atomOverlap(0.0)
, score(0.0)
, rotor(4,0.0)
{
   rotor[0] = 1.0;
}


		
SolutionInfo::~SolutionInfo(void)
{
}



void
SolutionInfo::printScores(Options& uo)
{
	*(uo.scoreOutStream) << dbName
		<< "\t" << refName
		<< "\t" << std::setprecision(3) << atomOverlap / (refAtomVolume + dbAtomVolume - atomOverlap)
		<< "\t" << std::setprecision(3) << atomOverlap / (0.95*refAtomVolume + 0.05*dbAtomVolume)
		<< "\t" << std::setprecision(3) << atomOverlap / (0.05*refAtomVolume + 0.95*dbAtomVolume)
		<< "\t" << std::setprecision(5) << atomOverlap
		<< "\t" << std::setprecision(5) << refAtomVolume
		<< "\t" << std::setprecision(5) << dbAtomVolume << std::endl;
	return;
}



void
updateSolutionInfo(SolutionInfo& s, AlignmentInfo& res, double score, GaussianVolume& gv)
{
	s.dbAtomVolume = gv.overlap;
	s.dbCenter = gv.centroid;
	s.dbRotation = gv.rotation;
	s.atomOverlap = res.overlap;
	s.score = score;
	s.rotor = res.rotor;
	return;
}



void
setAllScores(SolutionInfo& res)
{
   std::ostringstream ss;

   OpenBabel::OBPairData* label1 = new OpenBabel::OBPairData();
   ss.str("");
   label1->SetAttribute(tanimoto);
   ss << res.atomOverlap / (res.refAtomVolume + res.dbAtomVolume - res.atomOverlap);
   label1->SetValue(ss.str());
   res.dbMol.SetData(label1);

   OpenBabel::OBPairData* label2 = new OpenBabel::OBPairData();
   ss.str("");
   label2->SetAttribute(tversky_ref);
   ss << res.atomOverlap / (0.95*res.refAtomVolume + 0.05*res.dbAtomVolume);
   label2->SetValue(ss.str());
   res.dbMol.SetData(label2);

   OpenBabel::OBPairData* label3 = new OpenBabel::OBPairData();
   ss.str("");
   label3->SetAttribute(tversky_db);
   ss << res.atomOverlap / (0.05*res.refAtomVolume + 0.95*res.dbAtomVolume);
   label3->SetValue(ss.str());
   res.dbMol.SetData(label3);

	return;
}
