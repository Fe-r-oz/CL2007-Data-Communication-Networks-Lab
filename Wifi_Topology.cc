/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
 /*
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation;
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */
  
 #include "ns3/core-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/network-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/mobility-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/yans-wifi-helper.h"
 #include "ns3/ssid.h"
  
 // Network Topology
 //
 //   Wifi 10.1.3.0
 //                 AP
 //  *    *    *    *
 //  |    |    |    |    10.1.1.0
 // n5   n6   n7   n0 --------------  *    *   *     *
 //                   point-to-point  |    |    |    |
 //                                   n1   n2   n3   n4
 //                                     Wifi 10.1.2.0
  
 using namespace ns3;
  
 NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");
  
 int 
 main (int argc, char *argv[])
 {
   bool verbose = true;
   uint32_t nWifi = 3;
   uint32_t nWifi_1 = 3;
   bool tracing = false;
  
   CommandLine cmd (__FILE__);
   
   cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi_1);
   cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
   cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  
   cmd.Parse (argc,argv);
  
   // The underlying restriction of 18 is due to the grid position
   // allocator's configuration; the grid layout will exceed the
   // bounding box if more than 18 nodes are provided.
   if (nWifi > 18)
     {
       std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
       return 1;
     }
     if (nWifi_1 > 18)
     {
       std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
       return 1;
     }
  
   if (verbose)
     {
       LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
       LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
     }
  
   NodeContainer p2pNodes;
   p2pNodes.Create (2);
  
   PointToPointHelper pointToPoint;
   pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
   pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
   NetDeviceContainer p2pDevices;
   p2pDevices = pointToPoint.Install (p2pNodes);
  
  
   NodeContainer wifiStaNodes;
   wifiStaNodes.Create (nWifi);
   NodeContainer wifiApNode = p2pNodes.Get (0);
   
    
   NodeContainer wifiStaNodes_1;
   wifiStaNodes_1.Create (nWifi_1);
   NodeContainer wifiApNode_1 = p2pNodes.Get (0);
   
  
  
   YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
   YansWifiPhyHelper phy;
   phy.SetChannel (channel.Create ());
   
   WifiHelper wifi;
   wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  
   WifiMacHelper mac;
   Ssid ssid = Ssid ("ns-3-ssid");
   mac.SetType ("ns3::StaWifiMac",
                "Ssid", SsidValue (ssid),
                "ActiveProbing", BooleanValue (false));
  
   NetDeviceContainer staDevices;
   staDevices = wifi.Install (phy, mac, wifiStaNodes);
   
    mac.SetType ("ns3::ApWifiMac",
                "Ssid", SsidValue (ssid));
    
   NetDeviceContainer apDevices;
   apDevices = wifi.Install (phy, mac, wifiApNode);
   
   
   MobilityHelper mobility;
  
   mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue (0.0),
                                  "MinY", DoubleValue (0.0),
                                  "DeltaX", DoubleValue (5.0),
                                  "DeltaY", DoubleValue (10.0),
                                  "GridWidth", UintegerValue (3),
                                  "LayoutType", StringValue ("RowFirst"));
  
   mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
   mobility.Install (wifiStaNodes);
  
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (wifiApNode);
   
   
   YansWifiChannelHelper channel_1 = YansWifiChannelHelper::Default ();
   YansWifiPhyHelper phy_1;
   phy_1.SetChannel (channel_1.Create ());
  
   
   WifiHelper wifi_1;
   wifi_1.SetRemoteStationManager ("ns3::AarfWifiManager");
  
   WifiMacHelper mac_1;
   Ssid ssid_1 = Ssid ("ns-3-ssid");
   mac_1.SetType ("ns3::StaWifiMac",
                "Ssid", SsidValue (ssid_1),
                "ActiveProbing", BooleanValue (false));
  
   NetDeviceContainer staDevices_1;
   staDevices_1 = wifi_1.Install (phy_1, mac_1, wifiStaNodes_1);
  

   mac_1.SetType ("ns3::ApWifiMac",
                "Ssid", SsidValue (ssid_1));
    
   NetDeviceContainer apDevices1;
   apDevices1 = wifi.Install (phy_1, mac_1, wifiApNode_1);
   
   MobilityHelper mobility_1;
  
   mobility_1.SetPositionAllocator ("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue (0.0),
                                  "MinY", DoubleValue (0.0),
                                  "DeltaX", DoubleValue (5.0),
                                  "DeltaY", DoubleValue (10.0),
                                  "GridWidth", UintegerValue (3),
                                  "LayoutType", StringValue ("RowFirst"));
  
   mobility_1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
   mobility_1.Install (wifiStaNodes_1);
  
   mobility_1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility_1.Install (wifiApNode_1);
  
   InternetStackHelper stack;
  
  stack.Install(wifiApNode);
  stack.Install (wifiStaNodes);
  stack.Install(p2pNodes.Get(1));
  stack.Install (wifiStaNodes_1);
 
   Ipv4AddressHelper address;
  
   address.SetBase ("10.1.1.0", "255.255.255.0");
   
   Ipv4InterfaceContainer p2pInterfaces;
   p2pInterfaces = address.Assign (p2pDevices);
  
   Ipv4AddressHelper address1;
  
   address1.SetBase("10.1.2.0", "255.255.255.0");
   
   Ipv4InterfaceContainer wifi_interface;
   wifi_interface = address1.Assign (apDevices);
   wifi_interface = address1.Assign (staDevices);
      
   Ipv4AddressHelper address2;
  
   address2.SetBase("10.1.3.0", "255.255.255.0");
   Ipv4InterfaceContainer wifi_interface_1;
    
   wifi_interface_1 = address2.Assign (apDevices1);
   wifi_interface_1 = address2.Assign (staDevices_1);
     
   UdpEchoServerHelper echoServer (93);
  
   ApplicationContainer serverApps = echoServer.Install (wifiStaNodes.Get(1));
   serverApps.Start (Seconds (1.0));
   serverApps.Stop (Seconds (20.0));
  
  
   UdpEchoClientHelper echoClient (wifi_interface.GetAddress (1), 93);
   echoClient.SetAttribute ("MaxPackets", UintegerValue (4.0));
   echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
   
   ApplicationContainer clientApps = echoClient.Install (wifiStaNodes_1.Get (1));
   clientApps.Start (Seconds (2.0));
   clientApps.Stop (Seconds (20.0));
   
   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   
   Simulator::Stop (Seconds (20.0));
   if (tracing)
     {
       phy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy_1.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
       pointToPoint.EnablePcapAll ("third");
       phy.EnablePcap ("third", apDevices.Get (0));
       phy_1.EnablePcap ("third", apDevices1.Get (1)); 
     }
   Simulator::Run ();
   Simulator::Destroy ();
   return 0;
 }