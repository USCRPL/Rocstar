/* *******************************************************************
 * Rocstar Simulation Suite                                          *
 * Copyright@2015, Illinois Rocstar LLC. All rights reserved.        *
 *                                                                   *
 * Illinois Rocstar LLC                                              *
 * Champaign, IL                                                     *
 * www.illinoisrocstar.com                                           *
 * sales@illinoisrocstar.com                                         *
 *                                                                   *
 * License: See LICENSE file in top level of distribution package or *
 * http://opensource.org/licenses/NCSA                               *
 *********************************************************************/
/* *******************************************************************
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,   *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   *
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          *
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE CONTRIBUTORS OR           *
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   *
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE    *
 * USE OR OTHER DEALINGS WITH THE SOFTWARE.                          *
 *********************************************************************/
// $Id: Neutral_Reader.h,v 1.4 2008/12/06 08:43:24 mtcampbe Exp $

#ifndef NEUTRAL_READER_H
#define NEUTRAL_READER_H

#include "../Rocsurf/include/surfbasic.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstdio>

COM_EXTERN_MODULE( Rocin);

// This class provides an interface for reading in a surface mesh 
// from the ".smf" or ".obj" file.
class Neutral_Reader {
public:
  // Constructor from an MPI communicator and a scale factor.
  Neutral_Reader()  {}

  // Assuming the window has been created, read in the local panes from
  // given file and return the number of local panes.
  int read_mesh( const char *fname, const std::string &wname) {
    std::ifstream is( fname); 
    if (!is) {
      std::cerr << "Error: Could not open file " << fname << std::endl;
      exit(-1);
    }

    // Create the window if not yet exist
    int h=COM_get_window_handle(wname.c_str());
    if ( h<=0) COM_new_window( wname.c_str());

    std::cout << "Reading file " << fname << std::endl;

    // Search for NUMNP
    do {
      get_nextline( is, buf);      
    } while ( !is.eof() && std::string(buf).find( "NUMNP")==std::string::npos);

    std::cerr << "Buf is now \"" << buf << "\" before getting nn" << std::endl;
    get_nextline( is, buf);
    int t1, t2, t3, t4, nn, ne;
    std::sscanf( buf, "%d %d %d %d %d %d", &nn, &ne, &t1, &t2, &t3, &t4);

    // Search for NODAL COORDINAES
    do {
      get_nextline( is, buf);      
    } while ( !is.eof() && std::string(buf).find( "NODAL")==std::string::npos);

    std::cout << "Reading " << nn << " nodes and " 
	      << ne << " triangles " << std::endl;
    read_pane_coors( is, wname, nn);

    // Search for ELEMENTS
    do {
      get_nextline( is, buf);      
    } while ( !is.eof() && std::string(buf).find( "ELEMENTS")==std::string::npos);
    read_pane_elems( is, wname, ne);

    return 1;
  }

private:
  void get_nextline( std::istream &is, char *str) {
    str[0] = '\0';
    if ( is.eof()) { return; }
    is.getline( str, MAXLEN); 
  }

  // Read in coordinates of a pane into a window if local
  void read_pane_coors( std::istream &is, const std::string &wname, int nn) {
    COM_set_size( (wname+".nc").c_str(), 1, nn);
    SURF::Point_3<double> *ps;
    COM_allocate_array( (wname+".nc").c_str(), 1, &(void*&)ps);
    
    for ( int i=0; i<nn; ++i) {
      get_nextline( is, buf); 
      int t1;

      std::sscanf( buf, "%d %lf %lf %lf", &t1, &ps[i][0], &ps[i][1], &ps[i][2]);
      COM_assertion( t1==i+1);
    }
  }

  // Read in element connectivity of a pane into a window if local
  void read_pane_elems( std::istream &is, const std::string &wname, int ne){
    // Note: Only supports triangular meshes now.
    COM_set_size( (wname+".:t3:").c_str(), 1, ne);
    SURF::Vector_3<int> *es;
    COM_allocate_array( (wname+".:t3:").c_str(), 1, &(void*&)es);
    
    for (int i=0; i<ne; ++i) {
      int t1, t2, t3;

      get_nextline( is, buf); 
      std::sscanf( buf, "%d %d %d %d %d %d", &t1, &t2, &t3, 
		   &es[i][0], &es[i][1], &es[i][2]);
      COM_assertion( t1 == i+1 && t2==3 && t3==3);
    }
  }

private:
  enum  {MAXLEN=255};
  char  buf[MAXLEN+1];
};

#endif






