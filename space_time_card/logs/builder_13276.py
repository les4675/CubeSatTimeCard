# 2024-11-21T11:53:39.689600500
import vitis

client = vitis.create_client()
client.set_workspace(path="C:/22257/space_time_card")

platform = client.create_platform_component(name = "timecard_hardware",hw_design = "C:/22257/space_time_card/timecard_HW_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0")

platform = client.get_component(name="timecard_hardware")
status = platform.build()

comp = client.create_app_component(name="timecard_software",platform = "C:/22257/space_time_card/timecard_hardware/export/timecard_hardware/timecard_hardware.xpfm",domain = "standalone_ps7_cortexa9_0",template = "hello_world")

status = platform.build()

comp = client.get_component(name="timecard_software")
comp.build()

vitis.dispose()

vitis.dispose()

