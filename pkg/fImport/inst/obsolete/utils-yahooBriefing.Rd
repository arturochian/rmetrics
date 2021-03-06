\name{utils-yahooBriefing}


\alias{yahooBriefing}


\title{Import Briefings from Yahoo}


\description{
    
    Imports briefings from chart.yahoo.com.
   
}


\usage{ 
yahooBriefing(query, file = "tempfile", source = NULL, save = FALSE, try = TRUE) 
}


\arguments{

    \item{file}{
        a character string with filename, usually having extension 
        ".csv", where to save the downloaded data.
        }
    \item{query}{
        a character string, denoting the location of the data at the 
        web site.
        }
    \item{save}{
        a logical value, if set to TRUE the downloaded data file will 
        be stored under the path and file name specified by the
        string \code{file}. By default FALSE.
        }
    \item{source}{a character string setting the URL of the source.
        If \code{NULL}, then the URL will be set automatically to its
        default value.
        }
    \item{try}{
        a logical value, if set to TRUE the Internet access will be
        checked.
        }
        
}
  
  
\value{

    returns a data frame with briefings downloaded from yahoo's web site.
    
}


\note{

    This function is no longer supported. The reason for this
    are the too many changes on the Yhaoo web site which made
    the download very prone to many failures. Feel free to use
    the code here to adapt the function to your own needs.

}


\author{

    Diethelm Wuertz for the Rmetrics \R-port.
    
}


\references{

    Diethelm Wuertz, Yohan Chalabi, and Andrew Ellis, (2010);
    \emph{Financial Market Data for R/Rmetrics}, 
    Rmetrics eBook, Rmetrics Association and Finance Online, Zurich,
    www.rmetrics.org.
    
}


\keyword{data}

