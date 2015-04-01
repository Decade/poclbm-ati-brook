# poclbm-ati-brook
Bitcoin miner that uses ATI Brook+ instead of OpenCL. Not a practical program, but I need something for my portfolio.

Might as well be public domain. I think the original poclbm is public domain.

The reason for this project was that I was interested in GPU computing, but my computer only had a Radeon HD 3000-series (R600 architecture) GPU. There is no OpenCL support for it. Also, those were the days when GPU mining for Bitcoins was practical. So, I downloaded FIPS 180-2, the AMD APP SDK, and poclbm, and got it working.

Then I discovered that it's pretty slow, and I did some inadvisable changes that didn't make it any faster. Didn't make it slower, either. Then I discovered the profiler, and found why it's so slow.

The Radeon R600 is a VLIW processor, with the execution units in clusters of 5 stream processing units. 4 of them are simple units that can ADD or OR, and 1 of them is a transcendental unit that can bitshift. SHA256 involves a whole lot of bitshifting. The 4 simple units end up starved while waiting for that 1 unit to shift the bits. It's just impossible for the R600 to mine Bitcoins efficiently.

By this time, I didn't need the distraction anymore, so I published the code and abandoned it.
