\name{listDescription}


\alias{listDescription}


\title{Description File Listing}


\description{

    Lists the content of a description file. 
    
}


\usage{
listDescription(package, character.only = FALSE)
}


\arguments{

    \item{package}{
        a literal character or character string denoting the name of the
        package to be listed.
    }
    \item{character.only}{
        a logical indicating whether 'package' can be
        assumed to be character strings.
    }
    
}


\value{

    prints the description file.

}


\seealso{
    
    \code{\link{listFunctions}},
    \code{\link{listIndex}}.
    
}


\examples{
## listDescription -
   listDescription("fBasics")
}


\keyword{programming}

