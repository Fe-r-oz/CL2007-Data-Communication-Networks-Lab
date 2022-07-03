# CL2007-Data-Communication-Networks-Lab

// Network topology

// 

//        n2 n3 n4              .

//         \ | /                .

//          \|/                 .

//     n1--- n0---n5---n9          .

//          /|\                 .

//         / | \                .

//        n8 n7 n6              .



Star Topology with nCSMA
//

//        n2 n3 n4              .

//         \ | /                .

//          \|/        n9 n10 n11  n12     .
                       |   |   |    |
//     n1--- n0---n5---===============          .
                        10.1.2.0
//          /|\                 .

//         / | \                .

//        n8 n7 n6              .

//


//
nCSMA nodes on Both sides, communication from one side to another.

  //
    //                                10.1.1.0
    //           n7   n6   n5                          n2   n3   n4
    //           |    |    |       point-to-point       |    |   |
    //       ================== n0 -------------- n1 ================
    //        LAN 10.1.3.0                               LAN 10.1.2.0
    
    
    
    nCsma Nodes On Both sides with a P2P connection
   
    
    //                                10.1.1.0
    //           n7   n6   n5                          n2   n3----n4
    //           |    |    |       point-to-point       |    |   |
    //       ================== n0 -------------- n1 ================
    //        LAN 10.1.3.0                               LAN 10.1.2.0
    
    
    
    
    
