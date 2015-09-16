/*
 * Copyright (C) 2015 Yasumasa Suenaga
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "stdafx.h"
#include "imgproc.h"


// cv::findContours() will crash w/ x64 apps which is built by VS 2015.
// So I create the wrapper of cvFindContours().
void FindContoursWrapper(cv::Mat binImg, std::vector<std::vector<cv::Point>> *contours_vec, cv::Rect roi) {
	CvMemStorage *storage = cvCreateMemStorage();
	CvSeq *contours = NULL;
	IplImage iplBinImg = binImg;

	cvSetImageROI(&iplBinImg, cvRect(roi.x, roi.y, roi.width, roi.height));

	int num_contour = cvFindContours(&iplBinImg, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	int Cnt;
	CvSeq *itr = contours;
	for (Cnt = 0, itr = contours; Cnt < num_contour; Cnt++, itr = itr->h_next) {
		std::vector<cv::Point> contour;

		for (int Cnt2 = 0; Cnt2 < itr->total; Cnt2++) {
			CvPoint *point = CV_GET_SEQ_ELEM(CvPoint, itr, Cnt2);
			contour.push_back(cv::Point(point->x, point->y));
		}

		contours_vec->push_back(contour);
	}

	cvReleaseMemStorage(&storage);
}

IMGPROC_API int WINAPI FindFecals(const char *fname, char *tmpfname, double bin_thresh, float roi_percentage,
																		double skip_area_sz, float min_cluster_ratio) {
	cv::Mat color = cv::imread(fname, cv::IMREAD_COLOR);
	cv::Mat gray;
	cv::cvtColor(color, gray, cv::COLOR_BGR2GRAY);

	cv::Mat binImg(gray.size(), gray.type());
	cv::threshold(gray, binImg, bin_thresh, 255, cv::THRESH_BINARY_INV);

	int roiWidth = (float)binImg.size().width * roi_percentage / 100.0;
	int roiHeight = (float)binImg.size().height * roi_percentage / 100.0;
	int roiX = (binImg.size().width - roiWidth) / 2;
	int roiY = (binImg.size().height - roiHeight) / 2;
	cv::Rect roi(roiX, roiY, roiWidth, roiHeight);

	std::vector<std::vector<cv::Point>> image_contours;
	FindContoursWrapper(binImg, &image_contours, roi);

	std::vector<cv::RotatedRect> ellipses;
	float min_area = FLT_MAX;

	for (auto itr = image_contours.begin(); itr != image_contours.end(); itr++) {

		for (auto points = itr->begin(); points != itr->end(); points++) {
			points->x += roi.x;
			points->y += roi.y;
		}

		if ((itr->size() > 5) && // http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html
			(cv::contourArea(*itr) > skip_area_sz)) {
			cv::RotatedRect rect = cv::fitEllipse(*itr);
			ellipses.push_back(rect);

			if (rect.size.area() < min_area) {
				min_area = rect.size.area();
			}

		}

	}

	cv::Scalar red(0, 0, 255);
	cv::Scalar green(0, 255, 0);
	cv::Scalar blue(255, 0, 0);
	cv::Scalar yellow(0, 255, 255);

	cv::rectangle(color, roi, blue);

	int fecal_count = 0;

	for (auto ellip = ellipses.begin(); ellip != ellipses.end(); ellip++) {
		cv::Scalar draw_color;
		float area_sz = ellip->size.area();
		
		if (area_sz == min_area) {
			draw_color = yellow;
			fecal_count++;
		}
		else {
			float fecal_cluster = area_sz / min_area;

			if (fecal_cluster > min_cluster_ratio) {
				draw_color = green;
				fecal_count += (int)fecal_cluster;
			}
			else {
				draw_color = red;
				fecal_count++;
			}

		}

		cv::ellipse(color, *ellip, draw_color);
	}

	char tmppath[MAX_PATH];
	GetTempPathA(MAX_PATH, tmppath);
	LPSTR ext = PathFindExtensionA(fname);
	sprintf_s(tmpfname, MAX_PATH, "%s\\usafun-count%s", tmppath, ext);

	cv::imwrite(tmpfname, color);

	return fecal_count;
}
