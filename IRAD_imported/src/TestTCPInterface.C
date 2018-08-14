///
/// @file 
/// @ingroup irad_group
/// @brief TCP Interface Test utility
/// 
#include "UnixUtils.H"
#include "NetUtils.H"
#include "FDUtils.H"

namespace IRAD {


  /// 
  /// Test utility for the TCP Interface.
  /// 
  /// @param argc number of arguments
  /// @param argv command line token strings
  /// @returns 0 on success, non-zero otherwise
  ///
  int TestTCPInterface(int argc,char *argv[])
  {
    if(!argv[1])
      std::exit(1);
    std::string mode(argv[1]);
    if(mode == "server"){
      Sys::Net::Server server;
      server.SimpleInit();
      int mother_descriptor = server.Descriptor();
      int client_descriptor = server.Accept();
      std::cout << "Accepted connection on descriptor: " << client_descriptor
		<< std::endl;
      Sys::FDSetMan setman;
      setman.AddInDescriptor(mother_descriptor);
      setman.AddIODescriptor(client_descriptor);
      bool done = false;
      int nsend = 0;
      int nrecv = 0;
      bool do_output = false;
      bool do_input = false;
      Sys::fdistream clientInStr(client_descriptor);
      Sys::fdostream clientOutStr(client_descriptor);
      while(!done){
	int nio = setman.Select(4.0); // select timeout is 2 seconds
	//      std::cout << nio << " descriptors are ready for io." << std::endl;
	if(setman.ReadyForInput(client_descriptor)){
	  std::cout << "Client has input to process." << std::endl;
	  do_input = true;
	  std::cout << "Receiving message number " << ++nrecv 
		    << " from client." << std::endl;
	  std::string clientstring;
	  std::getline(clientInStr,clientstring);
	  std::cout << "Client sent: " << clientstring << std::endl;
	  if(clientstring.find("final") != std::string::npos)
	    done = true;
	  if(!done)
	    clientOutStr << "Server recvd " << nrecv << ", sent " 
			 << ++nsend << std::endl;
	  else
	    clientOutStr << "Server recvd " << nrecv << ", send " 
			 << ++nsend << " <final>" << std::endl;
	}
      }
    }
    else{
      if(!argv[2])
	std::exit(1);
      std::string host(argv[2]);
      Sys::Net::Client client;
      if(client.Connect(host)){
	std::cerr << "Client failed to connect to " << host << std::endl;
	std::exit(1);
      }
      std::cout << "CLient is connected to " << host << " on " 
		<< client.Descriptor() << std::endl;
      Sys::FDSetMan setman;
      setman.AddIODescriptor(client.Descriptor());
      Sys::fdostream fdOut(client.Descriptor());
      Sys::fdistream fdIn(client.Descriptor());
      fdOut << "I am the client, and this is my initial message." << std::endl;
      int nsend = 0;
      int nrecv = 0;
      int nlines = 0;
      bool done = false;
      bool done_sending = false;
      while(!done){
	setman.Select(2.0);
	bool recv_time = (setman.ReadyForInput(client.Descriptor()) > 0);
	bool send_time = (setman.ReadyForOutput(client.Descriptor()) > 0);
	if(recv_time){
	  std::cout << "Ready to receive message " << ++nrecv 
		    << " from server." << std::endl;
	  std::string line;
	  std::getline(fdIn,line);
	  if(!line.empty())
	    std::cout << "line(" << ++nlines << ") from server: " << line << std::endl;
	  else
	    std::cout << "No line received." << std::endl;
	  if(!done_sending){
	    std::cout << "Sending message " << ++nsend << " to server." << std::endl;
	    fdOut << "I am the client, and I am sending message number " << nsend
		  << std::endl;
	    if(nsend == 2)
	      done_sending = true;
	  }
	  else{
	    std::cout << "Sending final" << std::endl;
	    fdOut << "final" << std::endl;
	    done = true;
	  }
	}
      }
      std::cout << "These lines were left over: " << std::endl;
      std::string line;
      while(std::getline(fdIn,line))
	std::cout << line << std::endl;
      sleep(30);
    }
    std::exit(0);
  }
};

int 
main(int argc,char *argv[])
{
  return(IRAD::TestTCPInterface(argc,argv));
}
