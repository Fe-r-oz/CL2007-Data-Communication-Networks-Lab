#include "ns3/core-module.h"

#include "ns3/network-module.h"

#include "ns3/netanim-module.h"

#include "ns3/internet-module.h"

#include "ns3/point-to-point-module.h"

#include "ns3/applications-module.h"

#include "ns3/point-to-point-layout-module.h"

#include "ns3/ipv4-global-routing-helper.h"

 #include "ns3/csma-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Star"); 



int

main (int argc, char *argv[])

{

 bool verbose = true;

 uint32_t nCsma = 3;

  

      CommandLine cmd;

      cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);  

      cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

      cmd.Parse (argc,argv);

      if (verbose)

        {

          LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);

          LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

        }

      nCsma = nCsma == 0 ? 1 : nCsma;

  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);

  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

uint32_t nSpokes = 8;

 

  NodeContainer n5n9;

  n5n9.Create(1);

     NodeContainer csmaNodes;

      csmaNodes.Add (n5n9.Get (0));

      csmaNodes.Create (nCsma);

  NS_LOG_INFO ("Build star topology.");

 

  PointToPointHelper pointToPoint;

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));

  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  PointToPointStarHelper star (nSpokes, pointToPoint);

 

PointToPointHelper pointToPoint1;

  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));

  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));

 

  CsmaHelper csma;

  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));

   csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));  

 

NetDeviceContainer p2pDevices;

p2pDevices =pointToPoint1.Install (n5n9.Get(0), star.GetSpokeNode (5));

      NetDeviceContainer csmaDevices;

      csmaDevices = csma.Install (csmaNodes);

  NS_LOG_INFO ("Install internet stack on all nodes.");

 

  InternetStackHelper internet;

  star.InstallStack (internet);

internet.Install (csmaNodes);

Ipv4AddressHelper quickly;

  NS_LOG_INFO ("Assign IP Addresses.");

  star.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"));

quickly.SetBase("10.1.10.0", "255.255.255.0");

Ipv4InterfaceContainer n5n9interface;

n5n9interface=quickly.Assign(csmaDevices);

quickly.SetBase("10.1.20.0", "255.255.255.0");

Ipv4InterfaceContainer n5n9interface1;

n5n9interface1=quickly.Assign(p2pDevices);

 

UdpEchoServerHelper echoServer (93);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get(3));

  serverApps.Start (Seconds (1.0));

  serverApps.Stop (Seconds (20.0));

ApplicationContainer clientApps;

ApplicationContainer clientApps1;

//n1, n2, n3, n4 will have client apps installed as mentioned in question statement and they will send packets to n12

  for (uint32_t i = 1; i <5; ++i)

    {

      UdpEchoClientHelper echoClient (n5n9interface.GetAddress(3), 93);

      echoClient.SetAttribute ("MaxPackets", UintegerValue (1));

      echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));

      echoClient.SetAttribute ("PacketSize", UintegerValue (512));

      clientApps = echoClient.Install (star.GetSpokeNode (i));

  clientApps.Start (Seconds (1.0));

  clientApps.Stop (Seconds (10.0));

    }

// n6 n7 n8 will have client apps installed, n5 and n0 does not a mentioned in questions statement

    for (uint32_t i1 = 6; i1 <star.SpokeCount (); ++i1)

    {

      UdpEchoClientHelper echoClient1 (n5n9interface.GetAddress(3), 93);

      echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));

      echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));

      echoClient1.SetAttribute ("PacketSize", UintegerValue (512));

      clientApps1 = echoClient1.Install (star.GetSpokeNode (i1));

  clientApps1.Start (Seconds (10.0));

  clientApps1.Stop (Seconds (20.0));

    }