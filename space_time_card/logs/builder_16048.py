# 2024-11-26T12:58:17.749460800
import vitis

client = vitis.create_client()
client.set_workspace(path="C:/22257/space_time_card")

platform = client.get_component(name="timecard_hardware")
status = platform.build()

status = platform.build()

comp = client.get_component(name="timecard_software")
comp.build()

status = platform.update_hw(hw_design = "C:/22257/space_time_card/timecard_HW_wrapper.xsa")

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.create_app_component(name="DMA_TEST",platform = "C:/22257/space_time_card/timecard_hardware/export/timecard_hardware/timecard_hardware.xpfm",domain = "standalone_ps7_cortexa9_0")

status = platform.build()

comp = client.get_component(name="DMA_TEST")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="DMA_TEST")

vitis.dispose()

vitis.dispose()

