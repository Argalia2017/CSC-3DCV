﻿#include "export.h"

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

	Matrix solve_pnp (CREF<Array<Point2F>> point1 ,CREF<Array<Point3F>> point2 ,CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist) const override {
		const auto r1x = invoke ([&] () {
			std::vector<cv::Point2d> ret = std::vector<cv::Point2d> (point1.size ()) ;
			for (auto &&i : iter (0 ,point1.size ())) {
				const auto r2x = Vector (point1[i]) ;
				ret[i].x = r2x[0] ;
				ret[i].y = r2x[1] ;
			}
			return move (ret) ;
		}) ;
		const auto r3x = invoke ([&] () {
			std::vector<cv::Point3d> ret = std::vector<cv::Point3d> (point2.size ()) ;
			for (auto &&i : iter (0 ,point2.size ())) {
				const auto r4x = Vector (point2[i]) ;
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
		return move (ret) ;
	}

	Vector redistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) const override {
		const auto r1x = (mat_k[1] * point).homogenize () ;
		const auto r2x = redistortion_ray (dist ,r1x) ;
		const auto r3x = (mat_k[0] * r2x).projection () ;
		return r3x ;
	}

	Vector undistortion (CREF<DuplexMatrix> mat_k ,CREF<Array<FLT64>> dist ,CREF<Vector> point) const override {
		const auto r1x = (mat_k[1] * point).homogenize () ;
		Vector ret = r1x ;
		for (auto &&i : iter (0 ,10)) {
			const auto r2x = redistortion_ray (dist ,ret) ;
			const auto r3x = r1x - r2x ;
			if (MathProc::abs (r3x[0]) < FLT64 (1E-5))
				if (MathProc::abs (r3x[1]) < FLT64 (1E-5))
					break ;
			const auto r4x = redistortion_jac (dist ,ret) ;
			ret += r4x.inverse () * r3x ;
		}
		ret = (mat_k[0] * ret).projection () ;
		return move (ret) ;
	}

	Vector redistortion_ray (CREF<Array<FLT64>> dist ,CREF<Vector> point) const {
		const auto r1x = point ;
		const auto r2x = MathProc::square (r1x[0]) + MathProc::square (r1x[1]) ;
		const auto r3x = r2x * r2x ;
		const auto r4x = r3x * r2x ;
		const auto r5x = 1 + dist[0] * r2x + dist[1] * r3x + dist[4] * r4x ;
		const auto r6x = 2 * r1x[0] * r1x[1] ;
		const auto r7x = 2 * MathProc::square (r1x[0]) + r2x ;
		const auto r8x = 2 * MathProc::square (r1x[1]) + r2x ;
		const auto r9x = r1x[0] * r5x + dist[2] * r6x + dist[3] * r7x ;
		const auto r10x = r1x[1] * r5x + dist[2] * r8x + dist[3] * r6x ;
		return Vector (r9x ,r10x ,r1x[2] ,0) ;
	}

	Matrix redistortion_jac (CREF<Array<FLT64>> dist ,CREF<Vector> point) const {
		Matrix ret = Matrix::identity () ;
		const auto r1x = point ;
		const auto r2x = MathProc::square (r1x[0]) + MathProc::square (r1x[1]) ;
		const auto r3x = r2x * r2x ;
		const auto r4x = r3x * r2x ;
		const auto r5x = 1 + dist[0] * r2x + dist[1] * r3x + dist[4] * r4x ;
		const auto r6x = dist[0] + dist[1] * 2 * r2x + dist[4] * 3 * r3x ;
		const auto r7x = 2 * r1x[0] ;
		const auto r8x = 2 * r1x[1] ;
		ret[0][0] = r5x + r1x[0] * r6x * r7x + dist[2] * r8x + dist[3] * r7x * 3 ;
		ret[0][1] = r1x[0] * r6x * r8x + dist[2] * r7x + dist[3] * r8x ;
		ret[1][0] = r1x[1] * r6x * r7x + dist[2] * r7x + dist[3] * r8x ;
		ret[1][1] = r5x + r1x[1] * r6x * r8x + dist[2] * r8x * 3 + dist[3] * r7x ;
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