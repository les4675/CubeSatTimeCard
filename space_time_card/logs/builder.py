# 2024-11-26T14:24:42.503868500
import vitis

client = vitis.create_client()
client.set_workspace(path="C:/22257/space_time_card")

platform = client.get_component(name="timecard_hardware")
status = platform.update_hw(hw_design = "C:/22257/space_time_card/timecard_HW_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="timecard_software")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "C:/22257/space_time_card/timecard_HW_wrapper.xsa")

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "C:/22257/space_time_card/timecard_HW_wrapper.xsa")

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

domain = platform.get_domain(name="standalone_ps7_cortexa9_0")

status = domain.regenerate()

status = platform.build()

status = domain.regenerate()

status = domain.regenerate()

status = platform.build()

vitis.dispose()

