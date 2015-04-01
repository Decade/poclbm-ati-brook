#include "search_vectors.h"
#include <boost/python.hpp>
//#include <boost/python/numeric.hpp>

#include <numpy/arrayobject.h>
//#include <boost/python/implicit.hpp>
#include <iostream>

//#include <boost/cstdint.hpp>

//#define SIZE 8162
#define SIZE 16384
//#define SIZE 32768
// Use size for how many groups of 4 nonces to test at once.
// So 128 goes to 512 items

#define OUTPUT_SIZE 0x100
#define OUTPUT_MASK 0xff
using namespace boost::python;
class BitcoinMiner{
 private:
  unsigned int dim;
  unsigned int s;
  ::brook::Stream<uint> as, bs, cs, ds, es, fs, gs, hs,
    e2s, e1s, e0s, a2s, a1s, a0s,
    bases, W2s, W16s, W17s,
    t3partials, t2_2s;
  ::brook::Stream<uint4> outs;
  ::brook::Stream<uint4> founds;
  //::brook::Stream<uint4> incs;
  //__declspec(align(256))
  struct o{
    uint buf[SIZE*4+64];
  };
  struct o out;
  uint* output;
 public:  
  BitcoinMiner(): dim(1),s(SIZE),as(1,&dim), bs(1,&dim), cs(1,&dim), ds(1, &dim), es(1, &dim), fs(1, &dim), gs(1, &dim), hs(1, &dim),
		  e2s(1,&dim), e1s(1, &dim), e0s(1, &dim), a2s(1,&dim), a1s(1,&dim), a0s(1,&dim),
		  bases(1,&dim), W2s(1,&dim), W16s(1,&dim), W17s(1,&dim),
		  t3partials(1,&dim),
		  t2_2s(1,&dim),
		  outs(1,&s),founds(1,&dim){
    //boost::python::numeric::array::set_module_and_type("numpy","ndarray");
    output = (uint*)((size_t((void*)(out.buf))+255)&~255);
    /*
    uint inc[SIZE*4];
    for(int i = 0; i < SIZE*4;++i)
      inc[i] = i;
    incs.read(inc);
    incs.finish();
    */
  }
  BitcoinMiner(int is): dim(1),s(is),as(1,&dim), bs(1,&dim), cs(1,&dim), ds(1, &dim), es(1, &dim), fs(1, &dim), gs(1, &dim), hs(1, &dim),
			e2s(1,&dim), e1s(1, &dim), e0s(1, &dim), a2s(1,&dim), a1s(1,&dim), a0s(1,&dim),
			bases(1,&dim), W2s(1,&dim), W16s(1,&dim), W17s(1,&dim),
			t3partials(1,&dim),
			t2_2s(1,&dim),
			outs(1,&s),founds(1,&dim){
    output = (uint*)((size_t((void*)(out.buf))+255)&~255);
  }
    /*
  void search(const uint a, const uint b, const uint c, const uint d,
	      const uint e, const uint f, const uint g, const uint h,
	      const uint e2, const uint e1, const uint e0,
	      const uint a2, const uint a1, const uint a0,
	      const uint base,
	      const uint W2, const uint W16, const uint W17,
	      const uint t3partial, const uint t2_2){
    as.read(&a);bs.read(&b);cs.read(&c);ds.read(&d);
    es.read(&e);fs.read(&f);gs.read(&g);hs.read(&h);
    e2s.read(&e2);e1s.read(&e1);e0s.read(&e0);
    a2s.read(&a2);a1s.read(&a1);a0s.read(&a0);
    W2s.read(&W2);W16s.read(&W16);W17s.read(&W17);
    t3partials.read(&t3partial);t2_2s.read(&t2_2);
    struct o basesource;
    for(int i = 0; i < SIZE*4; i+= 4)
      basesource.buf[i]=base + i;
    bases.read(basesource.buf);
    sha256_search(as, bs, cs, ds, es, fs, gs, hs,
		  e2s, e1s, e0s,a2s, a1s, a0s,
		  bases, W2s, W16s,W17s,
		  t3partials, t2_2s, outs);
    return;
  }*/
  void finish(object output_buffer){
    uint found[4];
    founds.write(found);
    if(found[0]|found[1]|found[2]|found[3]){
      outs.write(output,"nocopy");
      PyObject* array = output_buffer.ptr();
      uint* data = (uint*)PyArray_GETPTR1(array,0);
      uint foundnum=0;
      for(int i = 0; i < s << 2; ++i)
	if(output[i])
	  //	  data[OUTPUT_SIZE] = data[(output.buf[i]>>2) & OUTPUT_MASK] = output.buf[i];
	  data[(foundnum++)&OUTPUT_MASK]=output[i];
      data[OUTPUT_SIZE]=foundnum>OUTPUT_SIZE?OUTPUT_SIZE:foundnum;
    }
    return;
  }
  void searchNumeric(object ostate,
		     object ostate2,
		     object obase,
		     object of) {
    /* Working way to get a scalar.
    PyObject* other = t2.ptr();
    const uint* buffer;
    Py_ssize_t buflen;
    std::cout << "part zero\n";
    PyObject_AsReadBuffer(other,(const void**)&buffer, &buflen);
    std::cout << "part one\n";
    uint con = *buffer;
    std::cout << con << std::endl;
    */
    /* Working way to get array
    PyObject* array = t2.ptr();
    uint* data = (uint*)PyArray_GETPTR1(array,0);
    std::cout << "part zero\n";
    std::cout << data[0] << std::endl;
    std::cout << data[1] << std::endl;
    std::cout << data[2] << std::endl;
    */
    PyObject* pstate = ostate.ptr();
    uint* state = (uint*)PyArray_GETPTR1(pstate,0);
    PyObject* pstate2 = ostate2.ptr();
    uint* state2 = (uint*)PyArray_GETPTR1(pstate2,0);
    PyObject* pf = of.ptr();
    uint* f = (uint*)PyArray_GETPTR1(pf,0);
    as.read(state);bs.read(state+1);cs.read(state+2);ds.read(state+3);
    es.read(state+4);fs.read(state+5);gs.read(state+6);hs.read(state+7);
    e2s.read(state2+1);e1s.read(state2+2);e0s.read(state2+3);
    a2s.read(state2+5);a1s.read(state2+6);a0s.read(state2+7);
    W2s.read(f);W16s.read(f+1);W17s.read(f+2);
    t3partials.read(f+3);t2_2s.read(f+4);
    /*    std::cout << "Ooh\n";
    PyObject* other = base.ptr();
    std::cout << "Aah\n";
    const uint* buffer;
    std::cout << "Yay\n";
    Py_ssize_t buflen;
    std::cout << "Hurray\n";
    PyObject_AsReadBuffer(other,(const void**)&buffer, &buflen);
    std::cout << "Wow\n";
    uint con = *buffer;
    std::cout << "Yikes\n";
    */
    PyObject* pbase = obase.ptr();
    uint base;
    PyArray_ScalarAsCtype(pbase,&base);
    bases.read(&base);
    /*
    struct o basesource;
    uint* basealign = (uint*)((size_t((void*)(basesource.buf))+255)&~255);
    for(int i = 0; i < SIZE; ++i)
      basealign[i]=base+i;
    bases.read(basealign,"nocopy");
    */
    as.finish();bs.finish();cs.finish();ds.finish();es.finish();fs.finish();gs.finish();hs.finish();
    e2s.finish();e1s.finish();e0s.finish();a2s.finish();a1s.finish();a0s.finish();
    bases.finish();W2s.finish();W16s.finish();W17s.finish();t3partials.finish();t2_2s.finish();
    sha256_search(as, bs, cs, ds, es, fs, gs, hs,
		  e2s, e1s, e0s,a2s, a1s, a0s,
		  bases, W2s, W16s,W17s,
		  t3partials, t2_2s, outs);
    found_solution(outs,founds);
    return;
  }
  void searchPrev(object obase){
    PyObject* pbase = obase.ptr();
    uint base;
    PyArray_ScalarAsCtype(pbase,&base);
    /*
    struct o basesource;
    uint* basealign = (uint*)((size_t((void*)(basesource.buf))+255)&~255);
    for(int i = 0; i < SIZE; ++i)
      basealign[i]=base+i;
      bases.read(basealign,"nocopy");*/
    bases.read(&base);
    bases.finish();
    sha256_search(as, bs, cs, ds, es, fs, gs, hs,
		  e2s, e1s, e0s,a2s, a1s, a0s,
		  bases, W2s, W16s,W17s,
		  t3partials, t2_2s, outs);
    found_solution(outs,founds);
    return;
  }
  void setWork(object ostate,
	       object ostate2,
	       object of){
    PyObject* pstate = ostate.ptr();
    uint* state = (uint*)PyArray_GETPTR1(pstate,0);
    PyObject* pstate2 = ostate2.ptr();
    uint* state2 = (uint*)PyArray_GETPTR1(pstate2,0);
    PyObject* pf = of.ptr();
    uint* f = (uint*)PyArray_GETPTR1(pf,0);
    as.read(state);bs.read(state+1);cs.read(state+2);ds.read(state+3);
    es.read(state+4);fs.read(state+5);gs.read(state+6);hs.read(state+7);
    e2s.read(state2+1);e1s.read(state2+2);e0s.read(state2+3);
    a2s.read(state2+5);a1s.read(state2+6);a0s.read(state2+7);
    W2s.read(f);W16s.read(f+1);W17s.read(f+2);
    t3partials.read(f+3);t2_2s.read(f+4);
    as.finish();bs.finish();cs.finish();ds.finish();es.finish();fs.finish();gs.finish();hs.finish();
    e2s.finish();e1s.finish();e0s.finish();a2s.finish();a1s.finish();a0s.finish();
    bases.finish();W2s.finish();W16s.finish();W17s.finish();t3partials.finish();t2_2s.finish();
    return;
  }
};
/*
static unsigned int dim = 1;
static unsigned int s = SIZE;
*/
BOOST_PYTHON_MODULE(brookminer){
  import_array();
  class_<BitcoinMiner>("miner", init<int>())
    .def("search", &BitcoinMiner::searchNumeric)
    .def("finish", &BitcoinMiner::finish)
    .def("s", &BitcoinMiner::searchPrev)
    .def("work", &BitcoinMiner::setWork)
    ;
}
/*
Compile command:
cl /I"\Program Files (x86)\ATI\ATI Brook+ 1.4.0_beta\sdk\include" /I\boost\include/boost-1_47 /I\python27\include /I\python27\Lib\site-packages\numpy\core\include /LD /MD /EHsc phatk.cpp /link brook.lib search_vectors /libpath:"\Program Files (x86)\ati\ATI Brook+ 1.4.0_beta\sdk\lib" /libpath:\Python27\libs /libpath:\boost\lib /out:brookminer.pyd
 */
