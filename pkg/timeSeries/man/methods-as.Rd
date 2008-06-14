\name{as}


\title{timeSeries Class, Coercion and Transformation}

\alias{as}
\alias{as.timeSeries}
\alias{as.timeSeries.default}
\alias{as.timeSeries.data.frame} 
\alias{as.timeSeries.character}
\alias{as.timeSeries.zoo} 

\alias{coerce,ANY,timeSeries-method}
\alias{coerce,data.frame,timeSeries-method}
\alias{coerce,character,timeSeries-method}
\alias{coerce,ANY,timeSeries-method}

\alias{as.vector.timeSeries}
\alias{as.numeric.timeSeries}
\alias{as.matrix.timeSeries}
\alias{as.data.frame.timeSeries}
\alias{as.ts.timeSeries}
\alias{as.logical.timeSeries}


\description{
  
    Functions and methods dealing with the coercion of
    'timeSeries' objects.       
    \cr
    
    Functions to create 'timeSeries' objects from other objects:
    
    \tabular{ll}{
      \code{as.timeSeries} \tab Generic function to convert an object to
      a 'timeSeries' object, \cr
      \code{as.timeSeries.default} \tab Returns the unchanged  object, \cr
    \code{as.timeSeries.numeric} \tab Converts from a numeric vector, \cr 
    \code{as.timeSseries.data.frame} \tab Converts from a numeric vector, \cr 
    \code{as.timeSeries.matrix} \tab Converts from a matrix, \cr 
    \code{as.timeSeries.ts} \tab Converts from an object of class 'ts', \cr
    \code{as.timeSeries.character} \tab Converts from a named demo file,
      \cr  
    \code{as.timeSeries.zoo} \tab Converts an object of class zoo. }
    
    Functions to transform 'timeSeries' objects into other objects: 
    
    \tabular{ll}{
    \code{as.vector.timeSeries} \tab Coerces a 'timeSeries' to a vector,
    \cr 
    \code{as.numeric.timeSeries} \tab Coerces a 'timeSeries' to numeric,
    \cr 
    \code{as.matrix.timeSeries} \tab Coerces a 'timeSeries' to a matrix,
    \cr 
    \code{as.data.frame.timeSeries} \tab Coerces a 'timeSeries' to a 
    data.frame, \cr  
    \code{as.ts.timeSeries} \tab S3: Coerces a 'timeSeries' to a 'ts'
    object. \cr
      \code{as.ts.timeSeries} \tab S3: Coerces a 'timeSeries' to a 'logical'
    object. } 
  
}


\usage{
\method{as.timeSeries}{default}(x, \dots) 
\method{as.timeSeries}{data.frame}(x, \dots) 
\method{as.timeSeries}{character}(x, \dots) 
\method{as.timeSeries}{zoo}(x, \dots) 

% \S4method{coerce}{ANY,timeSeries}(from, to)

\method{as.vector}{timeSeries}(x, mode = "any")
\method{as.numeric}{timeSeries}(x, \dots)
\method{as.matrix}{timeSeries}(x, \dots)
\method{as.data.frame}{timeSeries}(x, row.names = NULL, optional = NULL, \dots)
\method{as.ts}{timeSeries}(x, \dots)
\method{as.logical}{timeSeries}(x, \dots)
}


\arguments{
  

    \item{mode}{ 
        a character string giving an atomic mode or \code{"list"},
        or (not for 'vector') \code{"any"}.
    }
    \item{optional}{
        A logical value. If \code{TRUE}, setting row names and converting
        column names (to syntactic names) is optional.
    }
    \item{row.names}{
        \code{NULL} or a character vector giving the row names for the
        data frame. Missing values are not allowed.
    }
    \item{x}{ 
        an object which is coerced according to the generic function.
    } 
    \item{\dots}{
        arguments passed to other methods.
    }
  
}


\value{
    
    \code{as.timeSeries}\cr
    \cr
    returns a S4 object of class \code{timeSeries}.
    \cr
    
    \code{as.vector}\cr
    \code{as.numeric}\cr
    \code{as.data.frame}\cr
    \code{as.matrix}\cr
    \code{as.ts}
    \cr
    return depending on the generic function a numeric vector, a 
    data frame, a matrix, or an object of class \code{ts}.
  
}


\examples{
## data - timeSeries:
   # Create an artificial timeSeries object:
   myFinCenter <<- "GMT"
   charvec = timeCalendar()
   data = matrix(rnorm(12))
   TS = timeSeries(data, charvec, units = "RAND")
   TS

## As Vector:
   as.vector(TS)
   
## As Matrix or Data Frame:
   as.matrix(TS)
   as.data.frame(TS)
   
## As Univariate Object of Class 'ts':
   as.ts(TS)
}


\keyword{chron}