#include "export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct SolverProcLayout {} ;

class SolverProcImplHolder final implement Fat<SolverProcHolder ,SolverProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Buffer3<FLT64> encode_rotation (CREF<Matrix> mat_r) const override {
		Buffer3<FLT64> ret ;
		const auto r1x = Quaternion (mat_r) ;
		const auto r2x = r1x.vector () ;
		ret[0] = r2x[0] ;
		ret[1] = r2x[1] ;
		ret[2] = r2x[2] ;
		return move (ret) ;
	}

	Matrix decode_rotation (CREF<Buffer3<FLT64>> mat_r) const override {
		const auto r1x = Vector (mat_r[0] ,mat_r[1] ,mat_r[2] ,0) ;
		const auto r2x = Quaternion (r1x) ;
		return r2x.matrix () ;
	}

	Array<FLT64> encode_matrix (CREF<Matrix> mat_a) const override {
		Array<FLT64> ret = Array<FLT64> (16) ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = i.mX + i.mY * 4 ;
			ret[ix] = mat_a[i] ;
		}
		return move (ret) ;
	}

	Matrix decode_matrix (CREF<Array<FLT64>> mat_a) const override {
		assume (mat_a.size () == 16) ;
		Matrix ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = i.mY * 4 + i.mX ;
			ret[i] = mat_a[ix] ;
		}
		return move (ret) ;
	}

	Matrix solve_pnp (CREF<Array<Point2F>> point_2d ,CREF<Array<Point3F>> point_3d ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) const override {
		const auto r1x = invoke ([&] () {
			std::vector<cv::Point2d> ret = std::vector<cv::Point2d> (point_2d.size ()) ;
			for (auto &&i : iter (0 ,point_2d.size ())) {
				const auto r2x = Vector (point_2d[i]) ;
				ret[i].x = r2x[0] ;
				ret[i].y = r2x[1] ;
			}
			return move (ret) ;
		}) ;
		const auto r3x = invoke ([&] () {
			std::vector<cv::Point3d> ret = std::vector<cv::Point3d> (point_3d.size ()) ;
			for (auto &&i : iter (0 ,point_3d.size ())) {
				const auto r4x = Vector (point_3d[i]) ;
				ret[i].x = r4x[0] ;
				ret[i].y = r4x[1] ;
				ret[i].z = r4x[2] ;
			}
			return move (ret) ;
		}) ;
		const auto r5x = invoke ([&] () {
			cv::Mat ret = cv::Mat::zeros (3 ,3 ,CV_64FC1) ;
			ret.at<double> (0 ,0) = mat_k[0][0][0] ;
			ret.at<double> (1 ,1) = mat_k[0][1][1] ;
			ret.at<double> (0 ,2) = mat_k[0][0][2] ;
			ret.at<double> (1 ,2) = mat_k[0][1][2] ;
			ret.at<double> (2 ,2) = 1 ;
			return move (ret) ;
		}) ;
		const auto r6x = invoke ([&] () {
			cv::Mat ret = cv::Mat::zeros (5 ,1 ,CV_64FC1) ;
			ret.at<double> (0 ,0) = dist[0] ;
			ret.at<double> (1 ,0) = dist[1] ;
			ret.at<double> (2 ,0) = dist[2] ;
			ret.at<double> (3 ,0) = dist[3] ;
			ret.at<double> (4 ,0) = dist[4] ;
			return move (ret) ;
		}) ;
		auto rax = cv::Mat () ;
		auto rbx = cv::Mat () ;
		const auto r7x = cv::solvePnP (r3x ,r1x ,r5x ,r6x ,rax ,rbx ,false ,cv::SOLVEPNP_EPNP) ;
		assume (r7x) ;
		const auto r8x = invoke ([&] () {
			cv::Mat ret = cv::Mat::zeros (3 ,3 ,CV_64FC1) ;
			cv::Rodrigues (rax ,ret) ;
			return move (ret) ;
		}) ;
		Matrix ret = Matrix::zero () ;
		ret[0][0] = r8x.at<double> (0 ,0) ;
		ret[0][1] = r8x.at<double> (0 ,1) ;
		ret[0][2] = r8x.at<double> (0 ,2) ;
		ret[0][3] = rbx.at<double> (0) ;
		ret[1][0] = r8x.at<double> (1 ,0) ;
		ret[1][1] = r8x.at<double> (1 ,1) ;
		ret[1][2] = r8x.at<double> (1 ,2) ;
		ret[1][3] = rbx.at<double> (1) ;
		ret[2][0] = r8x.at<double> (2 ,0) ;
		ret[2][1] = r8x.at<double> (2 ,1) ;
		ret[2][2] = r8x.at<double> (2 ,2) ;
		ret[2][3] = rbx.at<double> (2) ;
		ret[3][3] = 1 ;
		ret = ret.inverse () ;
		return move (ret) ;
	}
} ;

exports CREF<OfThis<SharedRef<SolverProcLayout>>> SolverProcHolder::instance () {
	return memorize ([&] () {
		OfThis<SharedRef<SolverProcLayout>> ret ;
		ret.mThis = SharedRef<SolverProcLayout>::make () ;
		SolverProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SolverProcHolder> SolverProcHolder::hold (VREF<SolverProcLayout> that) {
	return VFat<SolverProcHolder> (SolverProcImplHolder () ,that) ;
}

exports CFat<SolverProcHolder> SolverProcHolder::hold (CREF<SolverProcLayout> that) {
	return CFat<SolverProcHolder> (SolverProcImplHolder () ,that) ;
}
} ;