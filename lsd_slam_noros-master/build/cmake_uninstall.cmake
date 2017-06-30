MESSAGE(STATUS "Uninstalling")
IF (NOT EXISTS "C:/Users/ChurWoong/Desktop/lsd_slam_noros-master/build/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"C:/Users/ChurWoong/Desktop/lsd_slam_noros-master/build/install_manifest.txt\"")
ENDIF(NOT EXISTS "C:/Users/ChurWoong/Desktop/lsd_slam_noros-master/build/install_manifest.txt")

FILE(READ "C:/Users/ChurWoong/Desktop/lsd_slam_noros-master/build/install_manifest.txt" files)
STRING(REPLACE "\n" ";" files "${files}")
FOREACH (file ${files})
    MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    IF (EXISTS "$ENV{DESTDIR}${file}")
        EXECUTE_PROCESS(
            COMMAND C:/Users/ChurWoong/Desktop/cmake-3.4.1-win32-x86/bin/cmake.exe -E remove "$ENV{DESTDIR}${file}"
            OUTPUT_VARIABLE rm_out
            RESULT_VARIABLE rm_retval
        )
        IF(NOT ${rm_retval} EQUAL 0)
            MESSAGE(FATAL_ERROR 
          "Problem when removing \"$ENV{DESTDIR}${file}\"")
        ENDIF (NOT ${rm_retval} EQUAL 0)
    ELSE (EXISTS "$ENV{DESTDIR}${file}")
        MESSAGE(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
    ENDIF (EXISTS "$ENV{DESTDIR}${file}")
ENDFOREACH(file)
