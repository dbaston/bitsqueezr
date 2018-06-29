clamp <- function(x, a, b) {
  min(b, max(a, x))
}

squeeze_bits <- function(x, digits, method='trim', decimal=FALSE) {
  stopifnot(is.numeric(x))
  stopifnot(is.numeric(digits))

  if (method == 'trim') {
    if (decimal) {
      .Call("C_trim_bits_dd", as.double(x), as.integer(digits))
    } else {
      .Call("C_trim_bits_sd", as.double(x), as.integer(digits))
    }
  } else if (method == 'pad') {
    if (decimal) {
      .Call("C_pad_bits_dd", as.double(x), as.integer(digits))
    } else {
      .Call("C_pad_bits_sd", as.double(x), as.integer(digits))
    }
  } else if (method == 'groom') {
    if (decimal) {
      .Call("C_groom_bits_dd", as.double(x), as.integer(digits))
    } else {
      .Call("C_groom_bits_sd", as.double(x), as.integer(digits))
    }
  }
}

bits_as_string <- function(d) {
  bits <- .Call("C_double_bits", as.double(d))
  paste0(
    substr(bits, 1, 1), " ",
    substr(bits, 2, 12), " ",
    substr(bits, 13, 64)
  )
}
