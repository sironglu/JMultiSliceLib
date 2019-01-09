//
// C++ header file: JMultiSliceLib.h
// Declaration of entry points for library JMultislice.lib
// (Implementation see JMultisliceLib.cpp)
//
//
// Copyright (C) 2018, 2019 - Juri Barthel (juribarthel@gmail.com)
// Copyright (C) 2018, 2019 - Forschungszentrum J�lich GmbH, 52425 J�lich, Germany
//
//
//
// JMultiSlice implements routines of running the multislice algorithm on CPU and GPU.
// Uses libraries cufft.lib; cudart_static.lib; libfftw3f-3.lib
//
// Link cufft.lib; cudart_static.lib; libfftw3f-3.lib with the main program.
// You need to provide a version of the cuda fft runtime library ...
// - CUDA version 9.0: cufft64_90.dll
//
/*
 * The following statement of license applies *only* to this header file,
 * and *not* to the other files distributed with JMultsliceLib or derived
 * therefrom:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
//
//
// SETUP-CHAIN (minimum, without physics)
// 1) JMS_SetGridSize
// 2) 2.1) JMS_PhaseGratingSetup,
//    2.2) JMS_ObjectSliceSetup,
//    2.3) JMS_PropagatorSetup,
//    2.4) JMS_DetectorSetup (to be called after JMS_ObjectSliceSetup)
// 3) JMS_SetPhaseGratingData (for each slices)
// 4) JMS_SetPropagatorData (for each propagators)
// 5) JMS_SetDetectorData (for each detectors)
// 6) JMS_InitCore (once)
// 7) JMS_SetIncidentWave (once)
//
// For each multislice and as often as required
// 1) JMS_OffsetIncidentWave
// 2) JMS_CPUMultislice or JMS_GPUMultislice
// 3) JMS_GetResult
//
// DE-INITIALIZATION-CHAIN (minimum, without physics)
// 1) JMS_Cleanup
//
//
#pragma once

#ifndef JMS_LIB

#define JMS_LIB

#define JMS_CODE_CPU			1
#define JMS_CODE_GPU			2

#define JMS_DETECT_INTEGRATED	0
#define JMS_DETECT_IMAGE		1
#define JMS_DETECT_DIFFRACTION	2
#define JMS_DETECT_WAVEREAL		4
#define JMS_DETECT_WAVEFOURIER	8

#define JMS_ACCMODE_NONE		0
#define JMS_ACCMODE_INTEGRATE	1



#include "JMultiSlice.h"


// *** Global Instance of CJMultislice ***
static CJMultiSlice JMS;

// *** Basic Interface ***

// get version number (sub versions as fractional part)
extern "C" __declspec(dllexport) float __stdcall GetJMSVersion(void);

// set code debug level
extern "C" __declspec(dllexport) int __stdcall SetJMSDebugLevel(int debuglevel);

// set random number generator seed used on JMS_InitCore() (0 = time based)
extern "C" __declspec(dllexport) int __stdcall SetJMSRNGSeed(int rngseed = 0);


// *** Physics Interface ***

// set high tension value in keV
extern "C" __declspec(dllexport) float __stdcall SetHighTension(float ht);

// get current high tension value in keV
extern "C" __declspec(dllexport) float __stdcall GetHighTension(void);

// get current electron wavelength in nm
extern "C" __declspec(dllexport) float __stdcall GetWaveLength(void);

// set physical grid size in nm
extern "C" __declspec(dllexport) void __stdcall SetSupercellSize(float *a0);

// set physical grid size in nm
extern "C" __declspec(dllexport) void __stdcall SetSupercellSizeABC(float a, float b, float c);

// Set the slice thickness in nm
// - islc: a slice index valid in the current phase grating setup
// - fthickness: the thickness of slice islc in nm
extern "C" __declspec(dllexport) void __stdcall SetSliceThickness(int islc, float fthickness);

// Turns the diffraction de-scan on or off
extern "C" __declspec(dllexport) void __stdcall DiffractionDescan(bool bActivate);

// Set the diffraction descan values for a specific calculation thread.
// Use the same beam tilts as in the probe tilt, the routine inverts it.
// Requires successful InitCore.
// - whichcode: flag code type, either _JMS_CODE_CPU or _JMS_CODE_GPU
// - ndescanx, ndescany: horizontal and vertical descan [pixel]
// - iThread: thread ID (for calls with whichcode == _JMS_CODE_CPU)
extern "C" __declspec(dllexport) void __stdcall SetDiffractionDescanN(int whichcode, int ndescanx, int ndescany, int iThread = 0);

// Set the diffraction descan values for a specific calculation thread.
// Use the same beam tilts as in the probe tilt, the routine inverts it.
// Requires successful InitCore.
// - whichcode: flag code type, either _JMS_CODE_CPU or _JMS_CODE_GPU
// - descanx, descany: horizontal and vertical descan [1/nm]
// - iThread: thread ID (for calls with whichcode == _JMS_CODE_CPU)
extern "C" __declspec(dllexport) void __stdcall SetDiffractionDescan(int whichcode, float descanx, float descany, int iThread = 0);

// Set the diffraction descan values for a specific calculation thread.
// Use the same beam tilts as in the probe tilt, the routine inverts it.
// Requires successful InitCore.
// - whichcode: flag code type, either _JMS_CODE_CPU or _JMS_CODE_GPU
// - descanx, descany: horizontal and vertical descan [mrad]
// - iThread: thread ID (for calls with whichcode == _JMS_CODE_CPU)
extern "C" __declspec(dllexport) void __stdcall SetDiffractionDescanMRad(int whichcode, float descanx, float descany, int iThread = 0);



// *** Numerics Interface ***

// set calculation grid size parameters (pre-set, no allocation)
extern "C" __declspec(dllexport) void __stdcall SetGridSize(int nx, int ny);

// get current calculation grid size
extern "C" __declspec(dllexport) void __stdcall GetGridSize(int &nx, int &ny);

// get number of registered detection slices
extern "C" __declspec(dllexport) int __stdcall GetDetetionSliceNum(void);

// get number of registered detectors
extern "C" __declspec(dllexport) int __stdcall GetDetNum(void);

// get number of available CPUs
extern "C" __declspec(dllexport) int __stdcall GetCPUNum(void);

// get number of available GPUs
extern "C" __declspec(dllexport) int __stdcall GetGPUNum(void);

// get GPU device name
extern "C" __declspec(dllexport) int __stdcall GetGPUName(int idev, char* name);

// get current GPU device ID
extern "C" __declspec(dllexport) int __stdcall GetCurrentGPU(void);

// set current GPU device ID
extern "C" __declspec(dllexport) int __stdcall SetCurrentGPU(int idev);

// get some statistics of a GPU device
extern "C" __declspec(dllexport) int __stdcall GetGPUStats(int idev, int &iCMajor, int &iCMinor, int &iMaxThread, int64_t &CUDAmemtotal, int64_t &CUDAmemfree);

// get some statistics of a GPU device
extern "C" __declspec(dllexport) int __stdcall GetGPUCores(int idev, int &nMultiProc, int &nCores, int& nMaxThreadPerProc);

// Returns a hash table to unscramble the Fourier space by means of index access.
// After unscrambling, the zero beam is on pixel (m_nscx/2, m_nscy/2).
// Use this hash like this: a_unscrambled[idx] = a_scrambled[phash[idx]];
// - phash: unsigned int list of indices in the order of their occurrance in
//          an unscrambled array (preallocated by the calling code)
extern "C" __declspec(dllexport) int __stdcall GetUnscrambleHash(UINT* phash);



// *** Setup Interface ***

// setup phase grating memory
extern "C" __declspec(dllexport) int __stdcall PhaseGratingSetup(int whichcode, int nx, int ny, int nslc, int nvarmax, int* nslcvar);

// setup object slice sequence
extern "C" __declspec(dllexport) int __stdcall ObjectSliceSetup(int nobjslc, int* objslc);

// setup propgator memory
extern "C" __declspec(dllexport) int __stdcall PropagatorSetup(int whichcode, int npro, int *proidx);

// setup detector data
extern "C" __declspec(dllexport) int __stdcall DetectorSetup(int whichcode, int ndetper, int ndetint, int imagedet, int nthreads_CPU = 1);

// initialize core
extern "C" __declspec(dllexport) int __stdcall InitCore(int whichcode, int nCPUthreads = 1);

// set the incoming wave
extern "C" __declspec(dllexport) int __stdcall SetIncidentWave(int whichcode, fcmplx* wav);

// set phase gratings for a structure slice
extern "C" __declspec(dllexport) int __stdcall SetSlicePhaseGratings(int whichcode, int islc, int nvar, fcmplx* pgr);

// set the propagator for a propagator index
extern "C" __declspec(dllexport) int __stdcall SetPropagatorData(int whichcode, int ipro, fcmplx* pro);

// set the detector function for a detector index
extern "C" __declspec(dllexport) int __stdcall SetDetectorData(int whichcode, int idet, float* det, int msklen=0, int* msk=NULL);



// *** Calculation Interface ***

// Calculates a STEM probe wavefunction in Fourier space using current parameters
// - prm: address of a CJProbeParams object defining physical probe parameters
// - wav: address receiving the probe wave function
extern "C" __declspec(dllexport) int __stdcall CalculateProbeWaveFourier(CJProbeParams* prm, fcmplx *wav);

// Ignores the backup wave function and stores wav directly in
// the wave function channel for the GPU multislice calculation.
// - wav: wave function in Fourier space
// - bTranspose: flag signalizing that the input wave function is transposed
extern "C" __declspec(dllexport) int __stdcall SetIncomingWaveGPU(fcmplx* wav, bool bTranspose = false);

// Ignores the backup wave function and stores wav directly in
// the wave function channel of thread iThread for the CPU multislice calculation.
// - wav: wave function in Fourier space
// - bTranspose: flag signalizing that the input wave function is transposed
// - iThread: CPU thread ID
extern "C" __declspec(dllexport) int __stdcall SetIncomingWaveCPU(fcmplx* wav, bool bTranspose = false, int iThread = 0);

// Calculates a propagator function for a given thickness, object tilt
// and current size and wavelength parameters. The propagator amplitudes
// will be scaled by 1./(m_nscx * m_nsxy) to preserve wave function norms
// during each multislice step.
// - fthick: propagation length in nm
// - otx, oty: object tilt parameters in deg
// - pro: address receiving the propagator grid
// - ntype: propagator type switch: 0 = planar, 1 = Fresnel
extern "C" __declspec(dllexport) int __stdcall CalculatePropagator(float fthick, float otx, float oty, fcmplx *pro, int ntype = 0);

// Calculates a ring detector function and writes it to a pre-allocated output
// array 'det' of size (m_nscx * m_nsxy). Also creates a mask if a pointer
// is provided to an array 'msk'. The function returns the length of the mask
// to 'msklen', but you should pre-allocate 'msk' to be able to hold (m_nscx * m_nsxy)
// items.
// - beta0, beta1: ring collection range in mrad
// - phi0, phi1: ring segment range in deg
// - theta0x, theta0y: ring center in mrad
// - sdsprofile: sensitivity profile file name
// - det: pointer to a float array receiving the detector function data (pre-allocated)
// - msklen: length of an access pixel mask list
// - msk: pointer to an access pixel mask list, which speeds up the detector readout, optional (pre-allocated)
extern "C" __declspec(dllexport) int __stdcall CalculateRingDetector(float beta0, float beta1, float phi0, float phi1, float theta0x, float theta0y, std::string sdsprofile, float *det, int &msklen, int *msk = NULL);

// Takes a copy of the backup wave function, offsets in (x, y, z), and stores
// in the the active wave function channel used for the multislice calculation.
// - whichcode: flag signaling which code to prepare (_JMS_CODE_CPU | _JMS_CODE_GPU)
// - dx, dy, dz: offset distances in 3 dimensions and nm units.
// - iThread: thread ID for CPU code, ignored for GPU code
extern "C" __declspec(dllexport) int __stdcall OffsetIncomingWave(int whichcode, float dx, float dy, float dz, int iThread = 0);

// Runs a multislice calculation on a CPU thread
// Assumes incident wave function present in _h_JMS_wav
// - islc0: index of the starting slice
// - accmode = 0: new accumulation, 1: accumulate to previous detections
// - iThread: thread ID
extern "C" __declspec(dllexport) int __stdcall CPUMultislice(int islc0, int accmode, float weight = 1.0f,  int iThread = 0);

// Runs a multislice calculation on GPU.
// Assumes incident wave function present in _d_JMS_wav.
// - islc0: index of the starting slice
// - accmode = 0: new accumulation, 1: accumulate to previous detections
extern "C" __declspec(dllexport) int __stdcall GPUMultislice(int islc0, int accmode, float weight=1.f);

// Copies detection results to provided host address
// - whichcode: flag signaling which code to prepare (_JMS_CODE_CPU | whichcode)
// - whichresult: flag signaling which result to retrieve, one of _JMS_DETECT_INTEGRATED, _JMS_DETECT_IMAGE, _JMS_DETECT_DIFFRACTION
// - dst: destination address recieving results.
// - iThread: thread ID of CPU code
extern "C" __declspec(dllexport) int __stdcall GetResult(int whichcode, int whichresult, float *dst, int iThread = 0);

// Clears host detector memory for a CPU thread
extern "C" __declspec(dllexport) int __stdcall ClearDetMem_h(int iThread);

// Clears device detector memory
extern "C" __declspec(dllexport) int __stdcall ClearDetMem_d(void);



// ** Cleanup Interface ***

// Cleans the complete setup (CPU & GPU) !!!
// - all arrays will be de-allocated on device and host
// - scalar values remain if not linked to the array handling
// - resets the setup status to NONE
extern "C" __declspec(dllexport) int __stdcall Cleanup(void);

// Cleans the FFTW module
// - call this only if all FFTW routines are halted and no longer used.
// - FFTW needs to be reinitialized afterwards.
extern "C" __declspec(dllexport) void __stdcall CleanFFTW(void);


#endif