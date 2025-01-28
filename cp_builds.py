import shutil
import sys

path = sys.argv[1]

shutil.copyfile(f"{path}/build/partition_table/partition-table.bin", f"{path}/builds/partition-table.bin")
shutil.copyfile(f"{path}/build/press_counter_robonomics.bin", f"{path}/builds/press_counter.bin")
shutil.copyfile(f"{path}/build/bootloader/bootloader.bin", f"{path}/builds/bootloader.bin")
