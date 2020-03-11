package cn.edu.seu.alumni_server.common.dto;

import cn.edu.seu.alumni_server.common.CONST;
import lombok.Data;

/**
 * controller 统一返回，包装对象.
 * @param <T>
 */
@Data
public class WebResponse<T> {
    Integer status;
    String message;
    T data;

    public WebResponse(Integer status, String message, T data) {
        this.status = status;
        this.message = message;
        this.data = data;
    }

    public WebResponse(WebResponseEnum webResponseEnum) {
        this.status = webResponseEnum.getStatus();
        this.message = webResponseEnum.getMessage();
        this.data = null;
    }

    public WebResponse(WebResponseEnum webResponseEnum, T data) {
        this.status = webResponseEnum.getStatus();
        this.message = webResponseEnum.getMessage();
        this.data = data;
    }

    /* 快捷方式 */
    public WebResponse() {
        this.status = CONST.SUCCESS_CODE;
        this.message = CONST.SUCCESS_MESSAGE_DEFAULT;
    }

    public WebResponse<T> success() {
        this.status = CONST.SUCCESS_CODE;
        this.message = CONST.SUCCESS_MESSAGE_DEFAULT;
        return this;
    }

    public WebResponse<T> success(T body) {
        this.status = CONST.SUCCESS_CODE;
        this.message = CONST.SUCCESS_MESSAGE_DEFAULT;
        this.data = body;
        return this;
    }

    public WebResponse<T> fail() {
        this.status = CONST.FAIL_CODE;
        this.message = CONST.SUCCESS_MESSAGE_DEFAULT;
        return this;
    }

    public WebResponse<T> fail(String message) {
        this.status = CONST.FAIL_CODE;
        this.message = message;
        return this;
    }

    public WebResponse<T> fail(String message, T body) {
        this.status = CONST.FAIL_CODE;
        this.message = message;
        this.data = body;
        return this;
    }
}
