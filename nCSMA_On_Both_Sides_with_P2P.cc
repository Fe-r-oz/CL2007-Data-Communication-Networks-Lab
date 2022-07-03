
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3-----n7
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3; // for the Right Hand Side
  
  
  uint32_t nCsma_1= 3; // For the Left Hand Side 

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma_1);

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
    
    //p2p connection between n3--n7 which also are csma nodes!! If we look carefully
    
      
      NodeContainer p2pNodes_one;
      p2pNodes_one.Create(2);
   


  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
    PointToPointHelper pointToPoint_one;
  pointToPoint_one.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint_one.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);
  
   NetDeviceContainer p2pDevices_one;
  p2pDevices_one = pointToPoint_one.Install (csmaNodes.Get(1), csmaNodes.Get(2));
//for p2p connection  between n3-n7

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
   stack.Install(p2pNodes_one);
  
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  
  
 
  Ipv4InterfaceContainer p2pInterfaces;
  
  p2pInterfaces = address.Assign (p2pDevices);
  
  address.SetBase ("10.1.4.0", "255.255.255.0");
  
   Ipv4InterfaceContainer p2pInterfaces_one;
   // since n3--n7 are also have point to point connection
  
  p2pInterfaces_one = address.Assign (p2pDevices_one);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
  
    Ipv4AddressHelper address_1;
      address_1.SetBase ("10.1.3.0", "255.255.255.0");
      Ipv4InterfaceContainer csmaInterfaces_1;
      csmaInterfaces_1 = address_1.Assign (csmaDevices_1);

  UdpEchoServerHelper echoServer (9);
  //installing the server on the right hand side

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  
  
  UdpEchoServerHelper echoServer1 (93);

  ApplicationContainer serverApps1 = echoServer1.Install (p2pNodes_one.Get (0));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (10.0));


  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (2), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
  
    UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (2),9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));


  ApplicationContainer clientApps = echoClient.Install (csmaNodes_1.Get(2));
  //Installing the client on the Left handside
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes_one.Get(1));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



  pointToPoint.EnablePcapAll ("second");
  csma.EnablePcap ("second", csmaDevices.Get (1), true);
  csma_1.EnablePcap ("second", csmaDevices_1.Get (0), true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
