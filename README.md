# JMultiSlice

JMultiSlice is C++ library code providing routines for fast multislice calculations
on multiple CPU and single GPU. Routine interfaces are delivered by classes.

A multislice algorithm [1] approximates the solution of the coupled system of
differential equations describing the dynamic electron diffraction (multiple
scattering) in a crystal for fast electrons. The approximation is based on
making discrete steps of the forward scattering along the main propagation
direction of the electrons. For each step, the 3d crystal scattering potential
is projected in a thin slice, assuming that the effect of each slice is a small
deflection of the electron. This allows to separate the scattering from the
propagation to the next slice for faster numerical calculation.


[1] J.M. Cowley and A.F. Moodie, Acta. Cryst. 10 (1957) p. 609.
    https://doi.org/10.1107/S0365110X57002194


Detailed reference to the approaches used in the code is given in [2].

[2] J. Barthel, Ultramic. 193 (2018) p. 1.
    https://doi.org/10.1016/j.ultramic.2018.06.003


## Usage

In your code, declare an object of CJMultiSlice and use its member functions to
setup calculation parameters.

Example (For demonstration of the approach. This is not meant to work without changes.)

```
// in "yourcode.h"
// ...
CJMultiSlice jms;
// ...

	
// in "yourcode.cpp"
// global setup
// - define which parts of the code to use
int whichcode = _JMS_CODE_GPU | _JMS_CODE_CPU;
jms.SetCurrentGPU(0); // select first GPU (if recognized)
jms.SetHighTension(fHT); // set high tension in kV
jms.SetGridSize(nx, ny); // set grid size in pixels
jms.SetSupercellSize(a, b, c); // set super-cell size in nm
// prepare object transmission functions (slices phase gratings)
// - int nz is the number of slices, nvarmax is the maximum of variants per slice
// - int* pnvar defines the number of variants for each slice individually
jms.PhaseGratingSetup(whichcode, nx, ny, nz, nvarmax, pnvar);
for (i = 0; i < nz; i++) { // set data for each slice in jms
	// Link phase gratings pData[i] to the calculation.
	// The phase gratings have to be calculated elsewhere.
	// Code to calculate this with JMultiSlice is on the TODO list.
	jms.SetPhaseGratingData(whichcode, i, pnvar[i], (fcmplx*)pData[i]);
}
// prepate the slice object stack (object structure along z)
// - nobjslc max. number of slices of the whole object, defines max. thickness
// - int* pobjslc is a list of slice IDs between 0 and nz.
jms.ObjectSliceSetup(nobjslc, pobjslc);
// prepare memory for the fre-space propagators between scattering slices
// - int npro = propagators to prepare, here npro = nz
// - int* proidx is a list of indices of length nz defining which propagator
//   should be used for each slice, {0,1,..,nz-1} in this example.
jms.PropagatorSetup(whichcode, npro, proidx);
// calculate propagators
for (i = 0; i < nz; i++) { // make one propagator for each slice
	// for each slice
	// float* fdz = list of slice thicknesses
	// float fotx, foty = sample tilt in degrees
	// float *ppro = propagator data buffer, prepare sufficiently
	jms.CalculatePropagator(fdz[i], fotx, foty, (fcmplx*)ppro[i]);
	// link the propagator to the calculation routines
	jms.SetPropagatorData(whichcode, i, (fcmplx*)ppro[i]);
}
// prepare the detector setup in JMS, this allocates on host and/or device
// - int nDetPer = detection period in number of slices -> thickness series
// - int nDetNum = number of integrating diffraction plane detectors
// - int nDetFlags = probe intensity readout options
//   none, any or an OR combination of _JMS_DETECT_IMAGE and
//   _JMS_DETECT_DIFFRACTION
// - int nCPU = number of CPU threads
njmserr = jms.DetectorSetup(whichcode, nDetPer, nDetNum, nDetFlags, nCPU);
// calculate and transfer detector functions
for (i = 0, i < nDetNum; i++) { // for each detector ...
	// calculate the detector function
	// - float beta0, beta1 (in) = annular range of detection [mrad]
	// - float phi0, phi1 (in) = azimuth range of det. (segment) [deg]
	// - float theta0x, theta0y (in) = annular center offset [mrad]
	// - std::string sprofile (n) = radial sensitivity profile
	//   (file name, "" or file not found sets sensitivity = 1)
	// - float* det[i] (out) = buffer receiving data, allocate sufficiently
	// - int msklen (out) = length of the detector function access mask
	// - int* msk (out) = access mask, allocate sufficiently
	//   NULL will cause non-masked access
	jms.CalculateRingDetector( beta0, beta1, phi0, phi1, theta0x, theta0y,
		                        sprofile, det[i], msklen, msk);
	// transfer the detector function
	jms.SetDetectorData(whichcode, i, det[i], msklen, msk);
}
// initialize the calculation cores
jms.InitCore(whichcode, nCPU)
// prepare the incident electron wave function (probe)
// - float* pwave = wave function calculated elsewhere
//   Code to calculate data of pwave with JMultiSlice is on the TODO list.
jms.SetIncidentWave(whichcode, (fcmplx*)pwave);
// ...
// write code for scanning or other type of experiments
// ...
// offset the incident probe (scan)
// - float fShiftX, fShiftY, fShiftZ = probe x, y shift and defocus [nm]
// - int iThread = thread ID in case of multiple CPU calculations
jms.OffsetIncomingWave(whichcode, fShiftX, fShiftY, fShiftZ, iThread);
// run either GPU or CPU code, depending in whichcode
// - int islc0 = starting object slice ID
// - int flgacc = accumulation mode, 0: none, 1: add to previous data
// - float fwgt = weight factor of this multislice (default: 1.0f)
jms.GPUMultislice(islc0, flgacc, fwgt);
// ... 
jms.CPUMultislice(islc0, flgacc, fwgt, iThread);
// ...
// get results
// int whichcode = either _JMS_CODE_GPU or _JMS_CODE_CPU
// int whichresult = either _JMS_DETECT_INTEGRATED, _JMS_DETECT_IMAGE,
//                      or _JMS_DETECT_DIFFRACTION
// float *dst = destination buffer receiving results with length and
//              shape depending on whichresult
jms->GetResult(whichcode, whichresult, dst, iThread);
// ...
// clean de-initialize
jms.Cleanup();
jms.CleanFFTW();
```


## Testing

A test program exists, testing the __stdcall interface JMultiSliceLib of the library.
More tests have been made with the class interface CJMultislice in the Dr. Probe
graphical user interface program,
[Dr. Probe website](http://www.er-c.org/barthel/drprobe/).


## Further development

The following points could be developed
* test new functions for probe generation
* interface for wave function readout
* householding of elastic wave functions for separating thermal diffuse scattering
* calculating plane, probe, and vortex wave functions
* calculating object transmission functions from structure files, including structure data I/O
* calculation of transition potentials for inelastic scattering
