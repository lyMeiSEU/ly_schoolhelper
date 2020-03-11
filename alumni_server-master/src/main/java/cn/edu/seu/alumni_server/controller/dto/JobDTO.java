package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Job;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class JobDTO {
    private Long accountId;
    private Long jobId;
    private String company;
    private String position;
    private Long startTime;
    private Long endTime;

    public JobDTO() {
    }

    public JobDTO(Job job) {
        BeanUtils.copyProperties(job, this);
    }

    public Job toJob() {
        Job job = new Job();
        BeanUtils.copyProperties(this, job);
        return job;
    }
}