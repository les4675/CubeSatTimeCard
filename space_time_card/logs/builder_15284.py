# 2024-11-26T12:47:15.283867100
import vitis

client = vitis.create_client()
client.set_workspace(path="C:/22257/space_time_card")

platform = client.get_component(name="timecard_hardware")
status = platform.build()

comp = client.get_component(name="timecard_software")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

vitis.dispose()

