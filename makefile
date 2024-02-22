##
d1=../util/
d2=../odbc/
d3=../map/
d4=../i_epic_database/
d5=../i_swat_database/
d6=../weather/
BINARIES = wxswat

CXXFLAGS_STANDARD = -g `wx-config --cppflags`
# -D UNICODE
CXXFLAGS_TDC = -D _ODBC_TDC -D UNICODE -D __WX__ -std=c++11 -m64
CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_TDC}

LDFLAGS_STANDARD = -g -m64
LDFLAGS = ${LDFLAGS_STANDARD} `wx-config --libs` -L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/odbc -lodbc -lmysqlclient


all: wxswat

dialog_configure.o: dialog_configure.cpp dialog_configure.h
	g++ ${CXXFLAGS} -c dialog_configure.cpp
dialog_open_database.o: dialog_open_database.cpp dialog_open_database.h
	g++ ${CXXFLAGS} -c dialog_open_database.cpp
dialog_control_record: dialog_control_record.cpp dialog_control_record.h
	g++ ${CXXFLAGS} -c dialog_control_record.cpp
dialog_hru: dialog_hru.cpp dialog_hru.h
	g++ ${CXXFLAGS} -c dialog_hru.cpp
dialog_subbasin: dialog_subbasin.cpp dialog_subbasin.h
	g++ ${CXXFLAGS} -c dialog_subbasin.cpp
notebook_control_record: notebook_control_record.cpp notebook_control_record.h
	g++ ${CXXFLAGS} -c notebook_control_record.cpp
notebook_hru: notebook_hru.cpp notebook_hru.h
	g++ ${CXXFLAGS} -c notebook_hru.cpp
notebook_subbasin: notebook_subbasin.cpp notebook_subbasin.h
	g++ ${CXXFLAGS} -c notebook_subbasin.cpp
FilesRecentlyOpened.o: FilesRecentlyOpened.cpp FilesRecentlyOpened.h
	g++ ${CXXFLAGS} -c FilesRecentlyOpened.cpp
frame_swat.o: frame_swat.cpp frame_swat.h
	g++ ${CXXFLAGS} -c frame_swat.cpp
gridwidget.o: gridwidget.cpp gridwidget.h
	g++ ${CXXFLAGS} -c gridwidget.cpp
list_control_records.o: list_control_records.cpp list_control_records.h
	g++ ${CXXFLAGS} -c list_control_records.cpp
page_control_record.o: page_control_record.cpp page_control_record.h
	g++ ${CXXFLAGS} -c page_control_record.cpp
page_control_details.o: page_control_details.cpp page_control_details.h
	g++ ${CXXFLAGS} -c page_control_details.cpp
page_control_weather.o: page_control_weather.cpp page_control_weather.h
	g++ ${CXXFLAGS} -c page_control_weather.cpp
page_file_variables_delimited.o: page_file_variables_delimited.cpp page_file_variables_delimited.h
	g++ ${CXXFLAGS} -c page_file_variables_delimited.cpp
page_hru.o: page_hru.cpp page_hru.h
	g++ ${CXXFLAGS} -c page_hru.cpp
page_hru_2.o: page_hru_2.cpp page_hru_2.h
	g++ ${CXXFLAGS} -c page_hru_2.cpp
page_hru_3.o: page_hru_3.cpp page_hru_3.h
	g++ ${CXXFLAGS} -c page_hru_3.cpp
page_subbasin.o: page_subbasin.cpp page_subbasin.h
	g++ ${CXXFLAGS} -c page_subbasin.cpp
page_subbasin_climate.o: page_subbasin_climate.cpp page_subbasin_climate.h
	g++ ${CXXFLAGS} -c page_subbasin_climate.cpp
page_subbasin_channel.o: page_subbasin_channel.cpp page_subbasin_channel.h
	g++ ${CXXFLAGS} -c page_subbasin_channel.cpp
page_subbasin_elevation.o: page_subbasin_elevation.cpp page_subbasin_elevation.h
	g++ ${CXXFLAGS} -c page_subbasin_elevation.cpp
page_subbasin_tributary.o: page_subbasin_tributary.cpp page_subbasin_tributary.h
	g++ ${CXXFLAGS} -c page_subbasin_tributary.cpp
swat_tree_control.o: swat_tree_control.cpp swat_tree_control.h
	g++ ${CXXFLAGS} -c swat_tree_control.cpp
wxSWAT.o: wxSWAT.cpp wxSWAT.h
	g++ ${CXXFLAGS} -c wxSWAT.cpp
EPICDeclarations.o: $(d4)EPICDeclarations.cpp $(d4)EPICDeclarations.h
	g++ ${CXXFLAGS} -c $(d4)EPICDeclarations.cpp
output_variable_description.o: $(d4)output_variable_description.cpp $(d4)output_variable_description.h
	g++ ${CXXFLAGS} -c $(d4)output_variable_description.cpp
control_record_report.o: $(d5)control_record_report.cpp $(d5)control_record_report.h
	g++ ${CXXFLAGS} -c $(d5)control_record_report.cpp
map_object_swat.o: $(d5)map_object_swat.cpp $(d5)map_object_swat.h
	g++ ${CXXFLAGS} -c $(d5)map_object_swat.cpp
output_column_position.o: $(d5)output_column_position.cpp $(d5)output_column_position.h
	g++ ${CXXFLAGS} -c $(d5)output_column_position.cpp
output_variable_description_swat.o: $(d5)output_variable_description_swat.cpp $(d5)output_variable_description_swat.h
	g++ ${CXXFLAGS} -c $(d5)output_variable_description_swat.cpp
output_yield_tracker.o: $(d5)output_yield_tracker.cpp $(d5)output_yield_tracker.h
	g++ ${CXXFLAGS} -c $(d5)output_yield_tracker.cpp
rowset_hru.o: $(d5)rowset_hru.cpp $(d5)rowset_hru.h
	g++ ${CXXFLAGS} -c $(d5)rowset_hru.cpp
rowset_subbasin.o: $(d5)rowset_subbasin.cpp $(d5)rowset_subbasin.h
	g++ ${CXXFLAGS} -c $(d5)rowset_subbasin.cpp
SWATBMPUrbanUse.o: $(d5)SWATBMPUrbanUse.cpp $(d5)SWATBMPUrbanUse.h
	g++ ${CXXFLAGS} -c $(d5)SWATBMPUrbanUse.cpp
SWATControlRecord.o: $(d5)SWATControlRecord.cpp $(d5)SWATControlRecord.h
	g++ ${CXXFLAGS} -c $(d5)SWATControlRecord.cpp
SWATCrop.o: $(d5)SWATCrop.cpp $(d5)SWATCrop.h
	g++ ${CXXFLAGS} -c $(d5)SWATCrop.cpp
SWATDeclarations.o: $(d5)SWATDeclarations.cpp $(d5)SWATDeclarations.h
	g++ ${CXXFLAGS} -c $(d5)SWATDeclarations.cpp
SWATDetentionPond.o: $(d5)SWATDetentionPond.cpp $(d5)SWATDetentionPond.h
	g++ ${CXXFLAGS} -c $(d5)SWATDetentionPond.cpp
SWATFertilizer.o: $(d5)SWATFertilizer.cpp $(d5)SWATFertilizer.h
	g++ ${CXXFLAGS} -c $(d5)SWATFertilizer.cpp
SWATLakeWaterQuality.o: $(d5)SWATLakeWaterQuality.cpp $(d5)SWATLakeWaterQuality.h
	g++ ${CXXFLAGS} -c $(d5)SWATLakeWaterQuality.cpp
SWATManagement.o: $(d5)SWATManagement.cpp $(d5)SWATManagement.h
	g++ ${CXXFLAGS} -c $(d5)SWATManagement.cpp
SWATOutputHRUAnnual.o: $(d5)SWATOutputHRUAnnual.cpp $(d5)SWATOutputHRUAnnual.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputHRUAnnual.cpp
SWATOutputHRUMonthly.o: $(d5)SWATOutputHRUMonthly.cpp $(d5)SWATOutputHRUMonthly.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputHRUMonthly.cpp
SWATOutputHSL.o: $(d5)SWATOutputHSL.cpp $(d5)SWATOutputHSL.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputHSL.cpp
SWATOutputReachAnnual.o: $(d5)SWATOutputReachAnnual.cpp $(d5)SWATOutputReachAnnual.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputReachAnnual.cpp
SWATOutputReachDaily.o: $(d5)SWATOutputReachDaily.cpp $(d5)SWATOutputReachDaily.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputReachDaily.cpp
SWATOutputReachMonthly.o: $(d5)SWATOutputReachMonthly.cpp $(d5)SWATOutputReachMonthly.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputReachMonthly.cpp
SWATOutputSubBasinAnnual.o: $(d5)SWATOutputSubBasinAnnual.cpp $(d5)SWATOutputSubBasinAnnual.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputSubBasinAnnual.cpp
SWATOutputSubBasinMonthly.o: $(d5)SWATOutputSubBasinMonthly.cpp $(d5)SWATOutputSubBasinMonthly.h
	g++ ${CXXFLAGS} -c $(d5)SWATOutputSubBasinMonthly.cpp
SWATPesticide.o: $(d5)SWATPesticide.cpp $(d5)SWATPesticide.h
	g++ ${CXXFLAGS} -c $(d5)SWATPesticide.cpp
SWATPointSources.o: $(d5)SWATPointSources.cpp $(d5)SWATPointSources.h
	g++ ${CXXFLAGS} -c $(d5)SWATPointSources.cpp
SWATPointSourcesAPEX.o: $(d5)SWATPointSourcesAPEX.cpp $(d5)SWATPointSourcesAPEX.h
	g++ ${CXXFLAGS} -c $(d5)SWATPointSourcesAPEX.cpp
SWATPointSourcesDaily.o: $(d5)SWATPointSourcesDaily.cpp $(d5)SWATPointSourcesDaily.h
	g++ ${CXXFLAGS} -c $(d5)SWATPointSourcesDaily.cpp
SWATPointSourcesMonthly.o: $(d5)SWATPointSourcesMonthly.cpp $(d5)SWATPointSourcesMonthly.h
	g++ ${CXXFLAGS} -c $(d5)SWATPointSourcesMonthly.cpp
SWATPond.o: $(d5)SWATPond.cpp $(d5)SWATPond.h
	g++ ${CXXFLAGS} -c $(d5)SWATPond.cpp
SWATPotentialEvapotranspiration.o: $(d5)SWATPotentialEvapotranspiration.cpp $(d5)SWATPotentialEvapotranspiration.h
	g++ ${CXXFLAGS} -c $(d5)SWATPotentialEvapotranspiration.cpp
SWATReservoir.o: $(d5)SWATReservoir.cpp $(d5)SWATReservoir.h
	g++ ${CXXFLAGS} -c $(d5)SWATReservoir.cpp
SWATRetentionIrrigationBasin.o: $(d5)SWATRetentionIrrigationBasin.cpp $(d5)SWATRetentionIrrigationBasin.h
	g++ ${CXXFLAGS} -c $(d5)SWATRetentionIrrigationBasin.cpp
SWATRouting.o: $(d5)SWATRouting.cpp $(d5)SWATRouting.h
	g++ ${CXXFLAGS} -c $(d5)SWATRouting.cpp
SWATSedimentationFiltrationBasin.o: $(d5)SWATSedimentationFiltrationBasin.cpp $(d5)SWATSedimentationFiltrationBasin.h
	g++ ${CXXFLAGS} -c $(d5)SWATSedimentationFiltrationBasin.cpp
SWATSeptic.o: $(d5)SWATSeptic.cpp $(d5)SWATSeptic.h
	g++ ${CXXFLAGS} -c $(d5)SWATSeptic.cpp
SWATSepticWaterQuality.o: $(d5)SWATSepticWaterQuality.cpp $(d5)SWATSepticWaterQuality.h
	g++ ${CXXFLAGS} -c $(d5)SWATSepticWaterQuality.cpp
SWATSoilLayers.o: $(d5)SWATSoilLayers.cpp $(d5)SWATSoilLayers.h
	g++ ${CXXFLAGS} -c $(d5)SWATSoilLayers.cpp
SWATSoils.o: $(d5)SWATSoils.cpp $(d5)SWATSoils.h
	g++ ${CXXFLAGS} -c $(d5)SWATSoils.cpp
SWATStreamWaterQuality.o: $(d5)SWATStreamWaterQuality.cpp $(d5)SWATStreamWaterQuality.h
	g++ ${CXXFLAGS} -c $(d5)SWATStreamWaterQuality.cpp
SWATTemporalConservationPractices.o: $(d5)SWATTemporalConservationPractices.cpp $(d5)SWATTemporalConservationPractices.h
	g++ ${CXXFLAGS} -c $(d5)SWATTemporalConservationPractices.cpp
SWATTillage.o: $(d5)SWATTillage.cpp $(d5)SWATTillage.h
	g++ ${CXXFLAGS} -c $(d5)SWATTillage.cpp
SWATUrban.o: $(d5)SWATUrban.cpp $(d5)SWATUrban.h
	g++ ${CXXFLAGS} -c $(d5)SWATUrban.cpp
SWATWatershedWaterQuality.o: $(d5)SWATWatershedWaterQuality.cpp $(d5)SWATWatershedWaterQuality.h
	g++ ${CXXFLAGS} -c $(d5)SWATWatershedWaterQuality.cpp
SWATWaterUse.o: $(d5)SWATWaterUse.cpp $(d5)SWATWaterUse.h
	g++ ${CXXFLAGS} -c $(d5)SWATWaterUse.cpp
SWATWeatherByMonth.o: $(d5)SWATWeatherByMonth.cpp $(d5)SWATWeatherByMonth.h
	g++ ${CXXFLAGS} -c $(d5)SWATWeatherByMonth.cpp
SWATWeatherHistorical.o: $(d5)SWATWeatherHistorical.cpp $(d5)SWATWeatherHistorical.h
	g++ ${CXXFLAGS} -c $(d5)SWATWeatherHistorical.cpp
SWATWeatherStations.o: $(d5)SWATWeatherStations.cpp $(d5)SWATWeatherStations.h
	g++ ${CXXFLAGS} -c $(d5)SWATWeatherStations.cpp
SWATWeirStage.o: $(d5)SWATWeirStage.cpp $(d5)SWATWeirStage.h
	g++ ${CXXFLAGS} -c $(d5)SWATWeirStage.cpp
SWATWetPond.o: $(d5)SWATWetPond.cpp $(d5)SWATWetPond.h
	g++ ${CXXFLAGS} -c $(d5)SWATWetPond.cpp
swat_controller.o: $(d5)swat_controller.cpp $(d5)swat_controller.h
	g++ ${CXXFLAGS} -c $(d5)swat_controller.cpp
swat_output.o: $(d5)swat_output.cpp $(d5)swat_output.h
	g++ ${CXXFLAGS} -c $(d5)swat_output.cpp
swat_routing_node.o: $(d5)swat_routing_node.cpp $(d5)swat_routing_node.h
	g++ ${CXXFLAGS} -c $(d5)swat_routing_node.cpp
swat_weather_list.o: $(d5)swat_weather_list.cpp $(d5)swat_weather_list.h
	g++ ${CXXFLAGS} -c $(d5)swat_weather_list.cpp
utility_afx.o: $(d1)utility_afx.cpp $(d1)utility_afx.h
	g++ ${CXXFLAGS} -c $(d1)utility_afx.cpp
dynamic_string.o: $(d1)dynamic_string.cpp $(d1)dynamic_string.h
	g++ ${CXXFLAGS} -c $(d1)dynamic_string.cpp
list.o: $(d1)list.cpp $(d1)list.h
	g++ ${CXXFLAGS} -c $(d1)list.cpp
listbox_files.o: $(d1)listbox_files.cpp $(d1)listbox_files.h
	g++ ${CXXFLAGS} -c $(d1)listbox_files.cpp
utility.o: $(d1)utility.cpp $(d1)utility.h
	g++ ${CXXFLAGS} -c $(d1)utility.cpp
filename_struct.o: $(d1)filename_struct.cpp $(d1)filename_struct.h
	g++ ${CXXFLAGS} -c $(d1)filename_struct.cpp
ARMS_Declarations.o: $(d1)ARMS_Declarations.cpp $(d1)ARMS_Declarations.h
	g++ ${CXXFLAGS} -c $(d1)ARMS_Declarations.cpp
interface_tree.o: $(d1)interface_tree.cpp $(d1)interface_tree.h
	g++ ${CXXFLAGS} -c $(d1)interface_tree.cpp
interface_tree_wx.o: $(d1)interface_tree_wx.cpp $(d1)interface_tree_wx.h
	g++ ${CXXFLAGS} -c $(d1)interface_tree_wx.cpp
interface_window.o: $(d1)interface_window.cpp $(d1)interface_window.h
	g++ ${CXXFLAGS} -c $(d1)interface_window.cpp
interface_window_wx.o: $(d1)interface_window_wx.cpp $(d1)interface_window_wx.h
	g++ ${CXXFLAGS} -c $(d1)interface_window_wx.cpp
Timestamp.o: $(d1)Timestamp.cpp $(d1)Timestamp.h
	g++ ${CXXFLAGS} -c $(d1)Timestamp.cpp
custom.o: $(d1)custom.cpp $(d1)custom.h
	g++ ${CXXFLAGS} -c $(d1)custom.cpp
nri_point.o: $(d1)nri_point.cpp $(d1)nri_point.h
	g++ ${CXXFLAGS} -c $(d1)nri_point.cpp
exact_decimal.o: $(d1)exact_decimal.cpp $(d1)exact_decimal.h
	g++ ${CXXFLAGS} -c $(d1)exact_decimal.cpp
file_output.o: $(d1)file_output.cpp $(d1)file_output.h
	g++ ${CXXFLAGS} -c $(d1)file_output.cpp
CryptString.o: $(d1)CryptString.cpp $(d1)CryptString.h
	g++ ${CXXFLAGS} -c $(d1)CryptString.cpp
dialog_error_list.o: $(d1)dialog_error_list.cpp $(d1)dialog_error_list.h
	g++ ${CXXFLAGS} -c $(d1)dialog_error_list.cpp
odbc_database.o: $(d2)odbc_database.cpp $(d2)odbc_database.h
	g++ ${CXXFLAGS} -c $(d2)odbc_database.cpp
odbc_field_set.o: $(d2)odbc_field_set.cpp $(d2)odbc_field_set.h
	g++ ${CXXFLAGS} -c $(d2)odbc_field_set.cpp
odbc_database_access.o: $(d2)odbc_database_access.cpp $(d2)odbc_database_access.h
	g++ ${CXXFLAGS} -c $(d2)odbc_database_access.cpp
odbc_database_mysql.o: $(d2)odbc_database_mysql.cpp $(d2)odbc_database_mysql.h
	g++ ${CXXFLAGS} -c $(d2)odbc_database_mysql.cpp
odbc_database_sql_server.o: $(d2)odbc_database_sql_server.cpp $(d2)odbc_database_sql_server.h
	g++ ${CXXFLAGS} -c $(d2)odbc_database_sql_server.cpp
dynamic_map.o: $(d3)dynamic_map.cpp $(d3)dynamic_map.h
	g++ ${CXXFLAGS} -c $(d3)dynamic_map.cpp
map_scale.o: $(d3)map_scale.cpp $(d3)map_scale.h
	g++ ${CXXFLAGS} -c $(d3)map_scale.cpp
color_set.o: $(d3)color_set.cpp $(d3)color_set.h
	g++ ${CXXFLAGS} -c $(d3)color_set.cpp
averager_weighted.o: $(d6)averager_weighted.cpp $(d6)averager_weighted.h
	g++ ${CXXFLAGS} -c $(d6)averager_weighted.cpp
wxswat: dialog_configure.o dialog_hru.o dialog_subbasin.o dialog_open_database.o dialog_control_record.o notebook_control_record.o notebook_hru.o notebook_subbasin.o FilesRecentlyOpened.o frame_swat.o gridwidget.o list_control_records.o page_control_record.o page_control_details.o page_control_weather.o page_hru.o page_hru_2.o page_hru_3.o page_subbasin.o page_subbasin_climate.o page_subbasin_channel.o page_subbasin_elevation.o page_subbasin_tributary.o page_file_variables_delimited.o control_record_report.o swat_tree_control.o wxSWAT.o EPICDeclarations.o output_variable_description.o map_object_swat.o output_column_position.o output_variable_description_swat.o output_yield_tracker.o rowset_hru.o rowset_subbasin.o SWATBMPUrbanUse.o SWATControlRecord.o SWATCrop.o SWATDeclarations.o SWATDetentionPond.o SWATFertilizer.o SWATLakeWaterQuality.o SWATManagement.o SWATOutputHRUAnnual.o SWATOutputHRUMonthly.o SWATOutputHSL.o SWATOutputReachAnnual.o SWATOutputReachDaily.o SWATOutputReachMonthly.o SWATOutputSubBasinAnnual.o SWATOutputSubBasinMonthly.o SWATPesticide.o SWATPointSources.o SWATPointSourcesAPEX.o SWATPointSourcesDaily.o SWATPointSourcesMonthly.o SWATPond.o SWATPotentialEvapotranspiration.o SWATReservoir.o SWATRetentionIrrigationBasin.o SWATRouting.o SWATSedimentationFiltrationBasin.o SWATSeptic.o SWATSepticWaterQuality.o SWATSoilLayers.o SWATSoils.o SWATStreamWaterQuality.o SWATTemporalConservationPractices.o SWATTillage.o SWATUrban.o SWATWatershedWaterQuality.o SWATWaterUse.o SWATWeatherByMonth.o SWATWeatherHistorical.o SWATWeatherStations.o SWATWeirStage.o SWATWetPond.o swat_controller.o swat_output.o swat_routing_node.o swat_weather_list.o utility_afx.o custom.o dynamic_string.o list.o listbox_files.o utility.o filename_struct.o ARMS_Declarations.o interface_tree.o interface_tree_wx.o interface_window.o interface_window_wx.o Timestamp.o nri_point.o exact_decimal.o file_output.o CryptString.o dialog_error_list.o odbc_database.o odbc_field_set.o odbc_database_access.o odbc_database_mysql.o odbc_database_sql_server.o dynamic_map.o map_scale.o color_set.o averager_weighted.o
	g++ -o wxswat dialog_configure.o dialog_hru.o dialog_subbasin.o dialog_open_database.o dialog_control_record.o notebook_control_record.o notebook_hru.o notebook_subbasin.o FilesRecentlyOpened.o frame_swat.o gridwidget.o list_control_records.o page_control_record.o page_control_details.o page_control_weather.o page_file_variables_delimited.o page_hru.o page_hru_2.o page_hru_3.o page_subbasin.o page_subbasin_climate.o page_subbasin_channel.o page_subbasin_elevation.o page_subbasin_tributary.o control_record_report.o swat_tree_control.o wxSWAT.o EPICDeclarations.o output_variable_description.o map_object_swat.o output_column_position.o output_variable_description_swat.o output_yield_tracker.o rowset_hru.o rowset_subbasin.o SWATBMPUrbanUse.o SWATControlRecord.o SWATCrop.o SWATDeclarations.o SWATDetentionPond.o SWATFertilizer.o SWATLakeWaterQuality.o SWATManagement.o SWATOutputHRUAnnual.o SWATOutputHRUMonthly.o SWATOutputHSL.o SWATOutputReachAnnual.o SWATOutputReachDaily.o SWATOutputReachMonthly.o SWATOutputSubBasinAnnual.o SWATOutputSubBasinMonthly.o SWATPesticide.o SWATPointSources.o SWATPointSourcesAPEX.o SWATPointSourcesDaily.o SWATPointSourcesMonthly.o SWATPond.o SWATPotentialEvapotranspiration.o SWATReservoir.o SWATRetentionIrrigationBasin.o SWATRouting.o SWATSedimentationFiltrationBasin.o SWATSeptic.o SWATSepticWaterQuality.o SWATSoilLayers.o SWATSoils.o SWATStreamWaterQuality.o SWATTemporalConservationPractices.o SWATTillage.o SWATUrban.o SWATWatershedWaterQuality.o SWATWaterUse.o SWATWeatherByMonth.o SWATWeatherHistorical.o SWATWeatherStations.o SWATWeirStage.o SWATWetPond.o swat_controller.o swat_output.o swat_routing_node.o swat_weather_list.o utility_afx.o custom.o dynamic_string.o list.o listbox_files.o utility.o filename_struct.o ARMS_Declarations.o interface_tree.o interface_tree_wx.o interface_window.o interface_window_wx.o Timestamp.o nri_point.o exact_decimal.o file_output.o CryptString.o dialog_error_list.o odbc_database.o odbc_field_set.o odbc_database_access.o odbc_database_mysql.o odbc_database_sql_server.o dynamic_map.o map_scale.o color_set.o averager_weighted.o ${LDFLAGS}

clean:
	rm -f *.o {BINARIES}
