#include "export.h"

#include "../Detection/export.h"

#include <csc_end.h>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

namespace CSC3DCV {
struct PlaneRenderingLayout {
	Path mFramePath ;
	String<STR> mPose1File ;
	String<STR> mPose2File ;
	ImageShape mViewShape ;
	DuplexMatrix mViewMatK ;
	FLT64 mViewDepth ;
	DuplexMatrix mPose1MatV ;
	DuplexMatrix mPose2MatV ;
	Array<Line2F> mPose1Line2D ;
	Array<Line2F> mPose2Line2D ;
	Array<Line2F> mPoseTLine2D ;
	Array<Line3F> mPoseLine3D ;
	Image<Color3B> mDisplayImage ;
} ;

class PlaneRenderingImplHolder final implement Fat<PlaneRenderingHolder ,PlaneRenderingLayout> {
public:
	void initialize () override {
		self.mFramePath = Path (slice ("D:/Documents/C++/csc_3dcv/data/5/frame")) ;
		self.mPose1File = self.mFramePath.child (slice ("00000")) ;
		self.mPose2File = self.mFramePath.child (slice ("00001")) ;
	}

	void execute () override {
		sync_view_mat_k () ;
		work_pose_line3d () ;
		work_pose_image_near () ;
		save_near_ply () ;
	}

	void sync_view_mat_k () {
		self.mViewShape.mCX = 1920 ;
		self.mViewShape.mCY = 1080 ;
		self.mViewShape.mStep = 3 ;
		const auto r1x = FLT64 (45) * MATH_R ;
		const auto r2x = MathProc::atan (r1x ,FLT64 (2)) * 2 ;
		const auto r3x = FLT64 (self.mViewShape.mCX) * MathProc::inverse (r2x) ;
		const auto r4x = FLT64 (self.mViewShape.mCX) / 2 ;
		const auto r5x = FLT64 (self.mViewShape.mCY) / 2 ;
		self.mViewMatK = PerspectiveMatrix (r3x ,r3x ,r4x ,r5x) ;
		self.mViewDepth = FLT64 (200) ;
		self.mPose1MatV = TranslationMatrix (0 ,0 ,0) ;
		self.mPose2MatV = TranslationMatrix (0 ,0 ,1) ;
	}

	void work_pose_line3d () {
		self.mPose1Line2D = load_line_txt (self.mPose1File) ;
		self.mPose2Line2D = load_line_txt (self.mPose2File) ;
		self.mPoseLine3D = Array<Line3F> (self.mPose1Line2D.size ()) ;
		self.mPoseTLine2D = Array<Line2F> (self.mPose1Line2D.size ()) ;
		const auto r1x = self.mViewMatK[0] * self.mPose2MatV[1] ;
		for (auto &&i : self.mPose1Line2D.range ()) {
			const auto r2x = Vector (self.mPose1Line2D[i].mMin) ;
			const auto r3x = Vector (self.mPose1Line2D[i].mMax) ;
			const auto r4x = (self.mViewMatK[1] * r2x).normalize () ;
			const auto r5x = (self.mViewMatK[1] * r3x).normalize () ;
			const auto r6x = r4x * self.mViewDepth + Vector::axis_w () ;
			const auto r7x = r5x * self.mViewDepth + Vector::axis_w () ;
			const auto r8x = self.mPose1MatV[0] * r6x ;
			const auto r9x = self.mPose1MatV[0] * r7x ;
			self.mPoseLine3D[i].mMin = r8x.xyz () ;
			self.mPoseLine3D[i].mMax = r9x.xyz () ;
			const auto r10x = (r1x * r8x).projection () ;
			const auto r11x = (r1x * r9x).projection () ;
			self.mPoseTLine2D[i].mMin = r10x.xyz () ;
			self.mPoseTLine2D[i].mMax = r11x.xyz () ;
		}
	}

	Array<Line2F> load_line_txt (CREF<String<STR>> file) const {
		const auto r1x = String<STR>::make (file ,slice (".txt")) ;
		const auto r2x = FileProc::load_file (r1x) ;
		auto mReader = TextReader (Ref<RefBuffer<BYTE>>::reference (r2x)) ;
		const auto r3x = mReader.poll (TYPE<LENGTH>::expr) ;
		assume (r3x > 0) ;
		Array<Line2F> ret = Array<Line2F> (r3x) ;
		for (auto &&i : ret.range ()) {
			mReader >> GAP ;
			const auto r4x = mReader.poll (TYPE<INDEX>::expr) ;
			assume (r4x == i) ;
			mReader >> GAP ;
			mReader >> ret[i].mMin.mX ;
			mReader >> GAP ;
			mReader >> ret[i].mMin.mY ;
			mReader >> GAP ;
			mReader >> ret[i].mMax.mX ;
			mReader >> GAP ;
			mReader >> ret[i].mMax.mY ;
		}
		return move (ret) ;
	}

	void work_pose_image_near () {
		const auto r1x = String<STR>::make (self.mPose2File ,slice (".bmp")) ;
		self.mDisplayImage = ImageProc::load_image (r1x) ;
		auto rax = ImageProc::peek_image (self.mDisplayImage ,TYPE<cv::Mat>::expr) ;
		const auto r2x = cv::Scalar (0 ,255 ,0 ,255) ;
		for (auto &&i : self.mPoseTLine2D) {
			const auto r3x = cv::Point2i (VAL32 (i.mMin.mX) ,VAL32 (i.mMin.mY)) ;
			const auto r4x = cv::Point2i (VAL32 (i.mMax.mX) ,VAL32 (i.mMax.mY)) ;
			cv::line (rax ,r3x ,r4x ,r2x ,VAL32 (1) ,cv::LINE_8) ;
		}
		const auto r5x = String<STR>::make (self.mPose2File ,slice ("-near.bmp")) ;
		const auto r6x = StringProc::stra_from_strs (r5x) ;
		cv::imwrite ((&r6x[0]) ,rax) ;
	}

	void save_near_ply () {
		const auto r1x = String<STR>::make (self.mPose2File ,slice ("-near.ply")) ;
		auto mWriter = StreamFileTextWriter (r1x) ;
		mWriter.deref << slice ("ply") << GAP ;
		mWriter.deref << slice ("format ascii 1.0") << GAP ;
		const auto r2x = invoke ([&] () {
			LENGTH ret = 0 ;
			ret += 1 ;
			ret += self.mPoseLine3D.length () * 2 ;
			ret += self.mPose2Line2D.length () * 2 ;
			return move (ret) ;
		}) ;
		mWriter.deref << slice ("element vertex ") << r2x << GAP ;
		mWriter.deref << slice ("property float x") << GAP ;
		mWriter.deref << slice ("property float y") << GAP ;
		mWriter.deref << slice ("property float z") << GAP ;
		const auto r3x = invoke ([&] () {
			LENGTH ret = 0 ;
			ret += self.mPoseLine3D.length () ;
			ret += self.mPose2Line2D.length () ;
			//ret += self.mPoseLine3D.length () * 2 ;
			return move (ret) ;
		}) ;
		mWriter.deref << slice ("element edge ") << r3x << GAP ;
		mWriter.deref << slice ("property int vertex1") << GAP ;
		mWriter.deref << slice ("property int vertex2") << GAP ;
		mWriter.deref << slice ("end_header") << GAP ;
		if ifdo (TRUE) {
			const auto r4x = self.mPose2MatV[0] * Vector::axis_w () ;
			mWriter.deref << r4x[0] << slice (" ") ;
			mWriter.deref << r4x[1] << slice (" ") ;
			mWriter.deref << r4x[2] << GAP ;
		}
		if ifdo (TRUE) {
			for (auto &&k : self.mPoseLine3D.range ()) {
				const auto r5x = Vector (self.mPoseLine3D[k].mMin) ;
				const auto r6x = Vector (self.mPoseLine3D[k].mMax) ;
				mWriter.deref << r5x[0] << slice (" ") ;
				mWriter.deref << r5x[1] << slice (" ") ;
				mWriter.deref << r5x[2] << GAP ;
				mWriter.deref << r6x[0] << slice (" ") ;
				mWriter.deref << r6x[1] << slice (" ") ;
				mWriter.deref << r6x[2] << GAP ;
			}
		}
		if ifdo (TRUE) {
			for (auto &&k : self.mPose2Line2D.range ()) {
				const auto r7x = Vector (self.mPose2Line2D[k].mMin) ;
				const auto r8x = Vector (self.mPose2Line2D[k].mMax) ;
				const auto r9x = self.mViewMatK[1] * r7x + Vector::axis_w () ;
				const auto r10x = self.mViewMatK[1] * r8x + Vector::axis_w () ;
				const auto r11x = self.mPose2MatV[0] * r9x ;
				const auto r12x = self.mPose2MatV[0] * r10x ;
				mWriter.deref << r11x[0] << slice (" ") ;
				mWriter.deref << r11x[1] << slice (" ") ;
				mWriter.deref << r11x[2] << GAP ;
				mWriter.deref << r12x[0] << slice (" ") ;
				mWriter.deref << r12x[1] << slice (" ") ;
				mWriter.deref << r12x[2] << GAP ;
			}
		}
		if ifdo (TRUE) {
			const auto r13x = 1 ;
			for (auto &&k : iter (0 ,self.mPoseLine3D.length ())) {
				const auto r14x = r13x + k * 2 + 0 ;
				const auto r15x = r13x + k * 2 + 1 ;
				mWriter.deref << r14x << slice (" ") ;
				mWriter.deref << r15x << GAP ;
			}
		}
		if ifdo (TRUE) {
			const auto r16x = 1 + self.mPoseLine3D.length () * 2 ;
			for (auto &&k : iter (0 ,self.mPose2Line2D.length ())) {
				const auto r17x = r16x + k * 2 + 0 ;
				const auto r18x = r16x + k * 2 + 1 ;
				mWriter.deref << r17x << slice (" ") ;
				mWriter.deref << r18x << GAP ;
			}
		}
		if ifdo (FALSE) {
			const auto r19x = 1 ;
			for (auto &&k : iter (0 ,self.mPoseLine3D.length ())) {
				const auto r20x = r19x + k * 2 + 0 ;
				const auto r21x = r19x + k * 2 + 1 ;
				mWriter.deref << 0 << slice (" ") ;
				mWriter.deref << r20x << GAP ;
				mWriter.deref << 0 << slice (" ") ;
				mWriter.deref << r21x << GAP ;
			}
		}
		mWriter.flush () ;
	}
} ;

exports OfThis<AutoRef<PlaneRenderingLayout>> PlaneRenderingHolder::create () {
	OfThis<AutoRef<PlaneRenderingLayout>> ret ;
	ret.mThis = AutoRef<PlaneRenderingLayout>::make () ;
	return move (ret) ;
}

exports VFat<PlaneRenderingHolder> PlaneRenderingHolder::hold (VREF<PlaneRenderingLayout> that) {
	return VFat<PlaneRenderingHolder> (PlaneRenderingImplHolder () ,that) ;
}

exports CFat<PlaneRenderingHolder> PlaneRenderingHolder::hold (CREF<PlaneRenderingLayout> that) {
	return CFat<PlaneRenderingHolder> (PlaneRenderingImplHolder () ,that) ;
}
} ;
