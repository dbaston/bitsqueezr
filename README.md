[![Travis build status](https://travis-ci.org/dbaston/bitsqueezr.svg?branch=master)](https://travis-ci.org/dbaston/bitsqueezr)
[![Coverage status](https://codecov.io/gh/dbaston/bitsqueezr/branch/master/graph/badge.svg)](https://codecov.io/github/dbaston/bitsqueezr?branch=master)
[![License](https://img.shields.io/badge/License-GPL%203.0-green.svg)](http://www.gnu.org/licenses/gpl-3.0.html)
[![CRAN](http://www.r-pkg.org/badges/version/bitsqueezr)](https://cran.r-project.org/package=bitsqueezr)
[![cran checks](https://cranchecks.info/badges/worst/bitsqueezr)](https://cran.r-project.org/web/checks/check_results_bitsqueezr.html)

# README

`bitsqueezr` is an R package providing a simple and fast (C-based) method of quantizing floating point numbers for precision-preserving compression, as implemented in the [netCDF Operators (NCO)](https://github.com/nco/nco) software package and described by [John Caron (2014)](https://www.unidata.ucar.edu/blogs/developer/en/entry/compression_by_bit_shaving) and [Charlie Zender (2016)](https://cloudfront.escholarship.org/dist/prd/content/qt7xd1739k/qt7xd1739k.pdf).

## Installation

`bitsqueezr` is available through CRAN and can be installed using `install.packages`:

    install.packages('bitsqueezr')
   
 The package has no external dependencies.
