import podio
from podio.root_io import Reader
import dd4hep as dd4hepModule
from ROOT import dd4hep
decoder = dd4hep.BitFieldCoder( "system:6,IsRightEndcap:2,x:12:-6,y:24:-6" )

# Open the EDM4hep ROOT file
filename = "kk.root"
reader = Reader(filename)

# Use the frame iterator explicitly
event_num = 0
for event in reader.get("events"):

    # Access the "MY_HITS"
    arc_collection = event.get("MY_HITS")

    # Iterate over the hits in the MY_HITS
    for hit in arc_collection:

        system_id=decoder.get( hit.getCellID(), "system")
        IsRightEndcap_id=decoder.get( hit.getCellID(), "IsRightEndcap")
        print(f"New hit\tsys id = {system_id}\tleft/right = {IsRightEndcap_id}")
