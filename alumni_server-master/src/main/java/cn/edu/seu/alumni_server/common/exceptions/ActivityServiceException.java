package cn.edu.seu.alumni_server.common.exceptions;

public class ActivityServiceException extends ServiceException {
    public ActivityServiceException() {
    }

    public ActivityServiceException(String message) {
        super(message);
    }

    public ActivityServiceException(String message, Throwable cause) {
        super(message, cause);
    }

    public ActivityServiceException(Throwable cause) {
        super(cause);
    }

    public ActivityServiceException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
