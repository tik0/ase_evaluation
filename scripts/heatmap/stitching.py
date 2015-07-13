#! /usr/bin/python3.4
"""Stitching

Usage:
	stitching.py <a_comma_seperated_list_of_targets> [data-dir]
"""

import sys

# get all textfiles
BASE_STRING = 'CAMERA_{}_tracking_data.txt'
ALL_CAMERA_DATA = []


def get_camera_data(camera, offset_x, offset_y, targets, datadir):
	with open(datadir + '/' + BASE_STRING.format(camera), 'r') as f:
		for line in f:
			elem_list = line.split(' ')
			elem_list[-1] = elem_list[-1].strip()
			if len(elem_list) == 1:
				pass
			elif len(elem_list) <= 3 or elem_list[1] in ['0', '']:
				pass
			else:
				to_append = []
				dct = {}
				target_items = elem_list[2::4]
				to_append = []
				first = True
				for index, target in enumerate(targets):
					if target not in target_items:
						pass
					else:
						if first:
							to_append += [elem_list[0], elem_list[1]]
							first = False
						dct[target] = target_items.index(target)
						helper = elem_list[(dct[target])*4+2:(dct[target])*4+6]
						helper[2] = str(int(helper[2]) + offset_x)
						helper[3] = str(int(helper[3]) + offset_y)
						to_append += helper

				if to_append:
					ALL_CAMERA_DATA.append(to_append)

if __name__ == '__main__':
	if len(sys.argv) < 2:
		sys.stderr.write("Usage: stitching.py <a_comma_seperated_list_of_targets> [data-dir]\n")
		sys.exit(1)

	targets = sys.argv[1].split(',')
	datadir = sys.argv[2] if len(sys.argv) > 2 else '.'

	sys.stderr.write("Targets: " + ' '.join(targets) + "\n")
	sys.stderr.write("Looking for data in " + datadir + "\n");

	get_camera_data(2, 0, 0, targets, datadir)
	get_camera_data(1, 1000, 0, targets, datadir)
	get_camera_data(3, 0, 1000, targets, datadir)
	get_camera_data(4, 1000, 1000, targets, datadir)

	ALL_CAMERA_DATA.sort(key=lambda line: float(line[0]))

	for line in ALL_CAMERA_DATA:
		print(' '.join(line))