@echo off

SET dir=src\apps\ui_src\appkit
SET app_c=%dir%\app.c
SET data_dir=%dir%\data
SET data_c=%data_dir%\ui_data.c

if not exist %app_c% (
	echo "Creating default app.c"
	md %dir%
	cd.>%app_c%
)

if not exist %data_c% (
	echo "Creating default ui_data.c"
	md %data_dir%
	echo const unsigned char ui_data[] = {0};>>%data_c%
)