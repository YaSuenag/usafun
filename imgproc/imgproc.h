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
#ifdef IMGPROC_EXPORTS
#define IMGPROC_API __declspec(dllexport)
#else
#define IMGPROC_API __declspec(dllimport)
#endif

extern "C" IMGPROC_API int WINAPI FindFecals(const char *fname, char *tmpfname, double bin_thresh, float roi_percentage,
																				double skip_area_sz, float min_cluster_ratio);
