# Platform creation script
# 1. Create new repository where the software shall reside and copy template files there
# 2. Copy hw hand off file fromVivado into hw_handoff file
# 3. Change pfm_name and hw_name variables accoringly
# 4. Open Vitis using the base folder of repository as workspace folder
# 5. Open xsct terminal and change dir to current workspace folder: `cd [getws]`
# 6. Execute script: `source ./scripts/mkpfm.tcl`
#

set origin_dir "."
set norm_origin [file normalize $origin_dir]
puts "\nCreate new Platform in folder $norm_origin"
set pfm_name "SPI_GPIO_PeripheryPfm"
set pfm_dir "$origin_dir/$pfm_name"
set hw_name "$origin_dir/hw_handoff/SPI-GPIO-Periphery.xsa"

# now create platform
platform create -name $pfm_name -hw $hw_name -proc {ps7_cortexa9_0} -os {standalone}
platform write
platform read "$pfm_dir/platform.spr"
platform active $pfm_name
platform generate

puts "Platform generation done"
