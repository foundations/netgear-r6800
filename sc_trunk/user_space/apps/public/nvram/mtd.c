/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

int mtd_erase(const char *mtd)
{
	int mtd_fd;
	mtd_info_t mtd_info;
	erase_info_t erase_info;

	/* Open MTD device */
	if ((mtd_fd = open(mtd, O_RDWR)) < 0) {
		return 1;
	}

	/* Get sector size */
	if (ioctl(mtd_fd, MEMGETINFO, &mtd_info) != 0) {
		close(mtd_fd);
		return 1;
	}

	erase_info.length = mtd_info.erasesize;

	for (erase_info.start = 0;
	     erase_info.start < mtd_info.size;
	     erase_info.start += mtd_info.erasesize) {
		if (ioctl(mtd_fd, MEMERASE, &erase_info) != 0) {
			close(mtd_fd);
			return 1;
		}
	}

	close(mtd_fd);
	return 0;
}

