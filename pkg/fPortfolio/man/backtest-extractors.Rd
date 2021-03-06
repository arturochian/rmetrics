\name{backtest-extractors}


\alias{getWindows.fPFOLIOBACKTEST}
\alias{getWindowsFun.fPFOLIOBACKTEST}
\alias{getWindowsParams.fPFOLIOBACKTEST}
\alias{getWindowsHorizon.fPFOLIOBACKTEST}
\alias{getStrategy.fPFOLIOBACKTEST}
\alias{getStrategyFun.fPFOLIOBACKTEST}
\alias{getStrategyParams.fPFOLIOBACKTEST}
\alias{getSmoother.fPFOLIOBACKTEST}
\alias{getSmootherFun.fPFOLIOBACKTEST}
\alias{getSmootherParams.fPFOLIOBACKTEST}                       
\alias{getSmootherLambda.fPFOLIOBACKTEST}
\alias{getSmootherDoubleSmoothing.fPFOLIOBACKTEST}
\alias{getSmootherInitialWeights.fPFOLIOBACKTEST}
\alias{getSmootherSkip.fPFOLIOBACKTEST}
\alias{getMessages.fPFOLIOBACKTEST}


\title{Portfolio backtest specification extractors}


\description{
  
    Extracts information from an object of class
    fPFOLIOBACKTEST. \cr
    
    The functions are:
    
    \tabular{ll}{
    \code{getWindows} \tab Extract windows slot, \cr       
    \code{getWindowsFun} \tab extract windows function, \cr    
    \code{getWindowsParams} \tab extract a list of windows specific parameters, \cr    
    \code{getWindowsHorizon} \tab extract windows horizon, \cr 
    \code{getStrategy} \tab extract strategy slot, \cr     
    \code{getStrategyFun} \tab extract the portfolio strategy function, \cr    
    \code{getStrategyParams} \tab extract a list of portfolio strategy specific parameters, \cr    
    \code{getSmoother} \tab extract the smoother slot, \cr     
    \code{getSmootherFun} \tab Extract the Ssoother function, \cr  
    \code{getSmootherParams} \tab extract a list of Smoothing specific parameters, \cr 
    \code{getSmootherLambda} \tab extract the smoothing parameter Lambda, \cr      
    \code{getSmootherDoubleSmoothing} \tab extract setting for double smoothing, \cr       
    \code{getSmootherInitialWeights} \tab extract the initial weights to used in the smoothing, \cr
    \code{getSmootherSkip} \tab extract the number of skipped months, \cr      
    \code{getMessages} \tab extract the message slot.}  
}
    

\usage{
\method{getWindows}{fPFOLIOBACKTEST}(object)
\method{getWindowsFun}{fPFOLIOBACKTEST}(object)
\method{getWindowsParams}{fPFOLIOBACKTEST}(object)
\method{getWindowsHorizon}{fPFOLIOBACKTEST}(object)

\method{getStrategy}{fPFOLIOBACKTEST}(object)
\method{getStrategyFun}{fPFOLIOBACKTEST}(object)
\method{getStrategyParams}{fPFOLIOBACKTEST}(object)

\method{getSmoother}{fPFOLIOBACKTEST}(object)
\method{getSmootherFun}{fPFOLIOBACKTEST}(object)
\method{getSmootherParams}{fPFOLIOBACKTEST}(object)
\method{getSmootherLambda}{fPFOLIOBACKTEST}(object)
\method{getSmootherDoubleSmoothing}{fPFOLIOBACKTEST}(object)
\method{getSmootherInitialWeights}{fPFOLIOBACKTEST}(object)
\method{getSmootherSkip}{fPFOLIOBACKTEST}(object)

\method{getMessages}{fPFOLIOBACKTEST}(object)
}


\arguments{

    \item{object}{
        an object of class \code{fPFOLIOBACKTEST} as returned by 
        function \code{portfolioBacktest}.
        }

}


\references{

W\"urtz, D., Chalabi, Y., Chen W., Ellis A. (2009);
    \emph{Portfolio Optimization with R/Rmetrics}, 
    Rmetrics eBook, Rmetrics Association and Finance Online, Zurich.
    
}


\keyword{models}

