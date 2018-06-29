\name{bits_as_string}
\alias{bits_as_string}

\title{Generate a text representation of the bits in a double}

\description{
Generate a character vector of ones, zeros, and spaces, representing the bits in a double. Spaces are used to separate the sign bit, 11 exponent bits, and 52 fraction bits.
}

\usage{bits_as_string(d)}

\arguments{
\item{d}{a numeric value}
}

\examples{
# Output binary representation of pi
bits_as_string(pi)
# 0 10000000000 1001001000011111101101010100010001000010110100011000
}
