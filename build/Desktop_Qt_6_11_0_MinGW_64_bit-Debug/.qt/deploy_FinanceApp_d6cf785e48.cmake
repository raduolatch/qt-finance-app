include("C:/apkproject/FinanceApp/build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/FinanceApp-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtmultimedia")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/apkproject/FinanceApp/build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/FinanceApp.exe"
    GENERATE_QT_CONF
)
