package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.sql.Date;
import java.sql.Timestamp;

@Data
public class Comment implements Serializable {

    private Long commentId;

    private Long postId;

    private Long topCommentId;

    private Long fatherCommentId;

    private Long accountId;

    private String content;

    private Date cTime;

    private Boolean validStatus;

    private static final long serialVersionUID = 1L;
}
