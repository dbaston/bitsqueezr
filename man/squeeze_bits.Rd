\name{squeeze_bits}
\alias{squeeze_bits}

\title{Change insignificant bits of numeric values for improved compressibility}

\description{
Change insignificant bits of numeric values to zero or one, increasing the compressibility of files containing the values. Insignificant bits can be "trimmed" (set to zero), "padded" (set to one), or "groomed" (element-wise alternation between trimming and padding). A discussion of these schemes is provided by \cite{Zender, Charles (2016) {Statistically-accurate precision-preserving quantization with compression, evaluated in the netCDF operators}. Geoscientific Model Development 9(9)}. The file size reduction depends on the level of quantization and the compression algorithm used.
}

\usage{
squeeze_bits(x, digits, method = 'trim', decimal = FALSE)
}

\arguments{
\item{x}{a numeric vector}
\item{digits}{number of digits to preserve}
\item{method}{'trim' sets insignificant bits to zero, 'pad' sets insignificant bits to one, and 'groom' alternates between 'trim' and 'pad'}
\item{decimal}{if \code{TRUE}, \code{d} will be interpreted to refer to decimal digits rather than significant digits.}
}

\examples{
# Check file size reduction when retaining 6 siginificant digits
x <- runif(100)
raw <- tempfile(fileext='.rds')
quantized <- tempfile(fileext='.rds')

saveRDS(x, raw, compress='xz')
saveRDS(squeeze_bits(x, 6, method='trim'), quantized, compress='xz')

file.size(quantized) / file.size(raw)
# 0.6776316

# Display binary representation of pi with various levels of trimming
for (d in 1:15) {
  cat(bits_as_string(squeeze_bits(pi, d, method='trim')), '\n')
}

}

