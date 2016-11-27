#include "debug.h"

#if !defined(WITH_OPENCV)
int main() { Fatal("need opencv\n"); return 0; }
#else
#include <opencv2/highgui/highgui.hpp>

#include "imgproc.h"
#include "eigen.h"
#include "timer.h"

int main()
{
  auto I = cv::imread("/home/halismai/lena_gray.png", cv::IMREAD_UNCHANGED);
  if(I.empty())
    Fatal("Failed to read image\n");

  cv::Mat_<float> Is(I.rows, I.cols);
  imsmooth(I.ptr<uint8_t>(), ImageSize(I.rows, I.cols), 15, 2.0, Is.ptr<float>());

  {
    auto t = TimeCode(10,
                      [&]() {imsmooth(I.ptr<uint8_t>(), ImageSize(I.rows, I.cols), 5, 2.0, Is.ptr<float>()); });
    printf("imsmooth time %g\n", t);
  }

  /*
  cv::Mat dimg;
  cv::convertScaleAbs(Is, dimg);
  cv::imshow("dimg", dimg);
  cv::waitKey(0);
  */

  EigenAlignedContainer_<Image_<float>> bp;
  computeBitPlanes(I.ptr<uint8_t>(), ImageSize(I.rows, I.cols), bp, 1.0, 1.5);

  {
    auto t = TimeCode(50, [&] {computeBitPlanes(I.ptr<uint8_t>(), ImageSize(I.rows, I.cols), bp, 1.0, 1.5);} );
    printf("bitplanes time %g\n", t);
  }

  for(size_t i = 0; i < bp.size(); ++i) {
    cv::Mat_<float> B1(I.rows, I.cols, bp[i].data());
    cv::Mat dimg;
    cv::convertScaleAbs(B1, dimg, 255);
    cv::imshow("dimg", dimg);
    cv::waitKey(0);
  }

  return 0;
}
#endif

