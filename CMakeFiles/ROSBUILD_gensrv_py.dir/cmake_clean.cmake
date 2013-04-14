FILE(REMOVE_RECURSE
  "srv_gen"
  "src/CamTrap_Viper/srv"
  "CMakeFiles/ROSBUILD_gensrv_py"
  "src/CamTrap_Viper/srv/__init__.py"
  "src/CamTrap_Viper/srv/_CvService.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
