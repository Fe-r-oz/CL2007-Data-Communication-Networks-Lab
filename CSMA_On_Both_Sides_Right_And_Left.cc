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
    #include "ns3/network-module.h"
    #include "ns3/csma-module.h"
    #include "ns3/internet-module.h"
    #include "ns3/point-to-point-module.h"
    #include "ns3/applications-module.h"
    #include "ns3/ipv4-global-routing-helper.h"
    
    // Default Network Topology
   //
    //                                    10.1.1.0
    //      n4  n3   n2   n1   n0 -------------- n1   n2   n3   n4
    //       |    |      |        |      point-to-point  |    |    |    |
    //       ===========                             ================
    //        LAN 10.1.3.0                                                    LAN 10.1.2.0
    
    
    using namespace ns3;
    
    NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");
    
    int 
    main (int argc, char *argv[])
    {
      bool verbose = true;
      uint32_t nCsma = 3;
      uint32_t nCsma_1= 3;
      CommandLine cmd;
      cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
      cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma_1);
      cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
    
      cmd.Parse (argc,argv);
    
      if (verbose)
        {
          LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
          LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
        }
    
      nCsma = nCsma == 0 ? 1 : nCsma;
      nCsma_1 = nCsma_1 == 0 ? 1 : nCsma_1;
    
      NodeContainer p2pNodes;
      p2pNodes.Create (2);
    
      NodeContainer csmaNodes;
      csmaNodes.Add (p2pNodes.Get (1));
      csmaNodes.Create (nCsma);
      
      NodeContainer csmaNodes_1;
      csmaNodes_1.Add (p2pNodes.Get (0));
      csmaNodes_1.Create (nCsma_1);
    
      PointToPointHelper pointToPoint;
      pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
      pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
      NetDeviceContainer p2pDevices;
      p2pDevices = pointToPoint.Install (p2pNodes);
    
      CsmaHelper csma;
      csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
     csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
     
      CsmaHelper csma_1;
      csma_1.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
     csma_1.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
    
      NetDeviceContainer csmaDevices;
      csmaDevices = csma.Install (csmaNodes);
      
      NetDeviceContainer csmaDevices_1;
      csmaDevices_1 = csma_1.Install (csmaNodes_1);
    
     
      
      InternetStackHelper stack;
      stack.Install (csmaNodes);
      stack.Install (csmaNodes_1);

      Ipv4AddressHelper address;
      address.SetBase ("10.1.1.0", "255.255.255.0");
      
  
      Ipv4InterfaceContainer p2pInterfaces;
      p2pInterfaces = address.Assign (p2pDevices);
    
      address.SetBase ("10.1.2.0", "255.255.255.0");
      
      
      Ipv4InterfaceContainer csmaInterfaces;
      csmaInterfaces = address.Assign (csmaDevices);
      
       Ipv4AddressHelper address_1;
      address_1.SetBase ("10.1.3.0", "255.255.255.0");
      Ipv4InterfaceContainer csmaInterfaces_1;
      csmaInterfaces_1 = address_1.Assign (csmaDevices_1);
      
   
      UdpEchoServerHelper echoServer (93);
    
      ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (2));
      serverApps.Start (Seconds (1.0));
      serverApps.Stop (Seconds (10.0));
    
      UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (2), 93);
      echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
     echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
     echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
   
     ApplicationContainer clientApps = echoClient.Install (csmaNodes_1.Get (2));
    clientApps.Start (Seconds (2.0));
     clientApps.Stop (Seconds (10.0));
   
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   
     pointToPoint.EnablePcapAll ("second");
     csma.EnablePcap ("second", csmaDevices.Get (1), true);
     csma_1.EnablePcap ("second", csmaDevices_1.Get (0), true);
   
     Simulator::Run ();
     Simulator::Destroy ();
     return 0;
  }
