package cn.edu.seu.alumni_server.common.exceptions;

public class AlumniCircleServiceException extends ServiceException {
    public AlumniCircleServiceException() {
    }

    public AlumniCircleServiceException(String message) {
        super(message);
    }

    public AlumniCircleServiceException(String message, Throwable cause) {
        super(message, cause);
    }

    public AlumniCircleServiceException(Throwable cause) {
        super(cause);
    }

    public AlumniCircleServiceException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
