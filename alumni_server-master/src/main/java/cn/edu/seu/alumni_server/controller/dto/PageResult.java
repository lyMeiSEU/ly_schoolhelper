package cn.edu.seu.alumni_server.controller.dto;

import lombok.Data;
import lombok.extern.slf4j.Slf4j;

import java.util.List;

@Data
@Slf4j
public class PageResult<T> {
    long count;
    List<T> list;

    public PageResult() {
    }

    public PageResult(long count, List<T> list) {
        this.count = count;
        this.list = list;
    }
}
