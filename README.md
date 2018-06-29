# README

`bitsqueezr` is an R package providing a simple and fast (C-based) method of quantizing floating point numbers for precision-preserving compression, as implemented in the [netCDF Operators (NCO)](https://github.com/nco/nco) software package and described by [John Caron (2014)](https://www.unidata.ucar.edu/blogs/developer/en/entry/compression_by_bit_shaving) and [Charlie Zender (2016)](https://cloudfront.escholarship.org/dist/prd/content/qt7xd1739k/qt7xd1739k.pdf).

## Installation

`bitsqueezr` is not in CRAN and can be installed using `devtools`:

    devtools::install_github('dbaston/bitsqueezr')
   
 The package has no external dependencies.
