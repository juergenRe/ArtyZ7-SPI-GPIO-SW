# create Testapplication

set origin_dir "."
set pfm_name "SPI_GPIO_PeripheryPfm"
set app_name "SPI_GPIO_Periphery_stand-alone"

puts "\nCreate Application $app_name on platform $pfm_name"

platform active $pfm_name

domain active {standalone_domain}

app create -name $app_name -template "Empty Application(C)"
app build -name $app_name

puts "\n Application $app_name created and built"




