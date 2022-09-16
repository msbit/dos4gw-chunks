# DOS/4GW Chunks

[DOS/4GW](https://en.wikipedia.org/wiki/DOS/4G) embedded executables (that I have observed) have the following layout:

* Initial [MZ executable](https://en.wikipedia.org/wiki/DOS_MZ_executable) stub
* 1 or more BW EXP payloads
* 1 [Linear Executable](https://moddingwiki.shikadi.net/wiki/Linear_Executable_(LX/LE)_Format) payload

This tool extracts the various chunks into files based on the basename of the input file.

For example, running on the shareware version of `DOOM.EXE` (MD5 `e2382b7dc47ae2433d26b6e6bc312999`) outputs the following files:

* `DOOM.00000000.mz`
* `DOOM.0000f2a4.bw`  (`4GWPRO.EXP`)
* `DOOM.00025214.le`

An interesting case is the actual `DOS4GW.EXE` loader (MD5 `ee4033581845b3b8d28c4b7287eb4fc2`), which doesn't contain an LE payload, but does contain multiple BW payloads:

* `DOS4GW.00000000.mz`
* `DOS4GW.0000f474.bw`  (`EXPLOAD.EXP`)
* `DOS4GW.000155f4.bw`  (`LINEXE.EXP`)
* `DOS4GW.00020754.bw`  (`INT31W.EXP`)
* `DOS4GW.00025164.bw`  (`WVMM.EXP`)
* `DOS4GW.00032514.bw`  (`DOS4GW.EXP`)
