package cn.edu.seu.alumni_server.common.exceptions;

// FINISHED
public class ActivityMemberServiceException extends ServiceException {

    public ActivityMemberServiceException() {
    }

    public ActivityMemberServiceException(String message) {
        super(message);
    }

    public ActivityMemberServiceException(String message, Throwable cause) {
        super(message, cause);
    }

    public ActivityMemberServiceException(Throwable cause) {
        super(cause);
    }

    public ActivityMemberServiceException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
