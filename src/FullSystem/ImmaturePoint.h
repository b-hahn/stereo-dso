/**
* This file is part of DSO.
* 
* Copyright 2016 Technical University of Munich and Intel.
* Developed by Jakob Engel <engelj at in dot tum dot de>,
* for more information see <http://vision.in.tum.de/dso>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* DSO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSO. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

 
#include "util/NumType.h"
 
#include "FullSystem/HessianBlocks.h"
namespace dso
{


struct ImmaturePointTemporaryResidual
{
public:
	ResState state_state;
	double state_energy;
	ResState state_NewState;
	double state_NewEnergy;
	FrameHessian* target;
};


enum ImmaturePointStatus {
	IPS_GOOD=0,					// traced well and good
	IPS_OOB,					// OOB: end tracking & marginalize!
	IPS_OUTLIER,				// energy too high: if happens again: outlier!
	IPS_SKIPPED,				// traced well and good (but not actually traced).
	IPS_BADCONDITION,			// not traced because of bad condition.
	IPS_UNINITIALIZED};			// not even traced once.


class ImmaturePoint
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	// static values
	float color[MAX_RES_PER_POINT];
	float weights[MAX_RES_PER_POINT];

	Mat22f gradH;
	Vec2f gradH_ev;
	Mat22f gradH_eig;
	float energyTH;
	float u,v;
	float u_stereo, v_stereo;  // u, v used to do static stereo matching
	FrameHessian* host;
	int idxInImmaturePoints;

	float quality;

	float my_type;

	std::array<uint8_t, 3> color_rgb ={{255,0,0}};
	std::array<uint8_t, 3> semantics_rgb ={{0,127,0}};

	float idepth_min;
	float idepth_max;
	float idepth_min_stereo;  // idepth_min used to do static matching
	float idepth_max_stereo;  // idepth_max used to do static matching
	float idepth_stereo;
	ImmaturePoint(int u_, int v_, FrameHessian* host_, float type, CalibHessian* HCalib);
	ImmaturePoint(float u_, float v_, FrameHessian* host_, CalibHessian* HCalib);
	~ImmaturePoint();

    ImmaturePointStatus traceStereo(FrameHessian* frame, Mat33f K, bool mode_right);
	ImmaturePointStatus traceOn(FrameHessian* frame, const Mat33f &hostToFrame_KRKi, const Vec3f &hostToFrame_Kt, const Vec2f &hostToFrame_affine, CalibHessian* HCalib, bool debugPrint=false);

	ImmaturePointStatus lastTraceStatus;
	Vec2f lastTraceUV;
	float lastTracePixelInterval;

	float idepth_GT;

	double linearizeResidual(
			CalibHessian *  HCalib, const float outlierTHSlack,
			ImmaturePointTemporaryResidual* tmpRes,
			float &Hdd, float &bd,
			float idepth);
	float getdPixdd(
			CalibHessian *  HCalib,
			ImmaturePointTemporaryResidual* tmpRes,
			float idepth);

	float calcResidual(
			CalibHessian *  HCalib, const float outlierTHSlack,
			ImmaturePointTemporaryResidual* tmpRes,
			float idepth);

private:
};

}

