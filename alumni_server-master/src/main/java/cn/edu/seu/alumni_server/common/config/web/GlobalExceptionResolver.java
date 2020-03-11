package cn.edu.seu.alumni_server.common.config.web;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * 全局Controller层异常处理类
 */
@ControllerAdvice
@Slf4j
public class GlobalExceptionResolver {

    @ExceptionHandler(Throwable.class)
    @ResponseBody
    public WebResponse handleException(Throwable e) {
        // 打印异常堆栈信息
        log.error(e.getMessage(), e);
        return new WebResponse(CONST.FAIL_CODE, e.getMessage(), e.getStackTrace().toString());
    }

}