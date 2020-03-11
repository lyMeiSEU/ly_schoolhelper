package cn.edu.seu.alumni_server.controller.dto.internalRecommend;

import cn.edu.seu.alumni_server.dao.entity.Post;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class PostDTO4Create {
    private Long accountId;
    private Long postId;
    private Integer type;
    private String jobName;
    private String industry;
    private String companyName;
    private String province;
    private Integer city;
    private Integer salaryRange;
    private String jobDetail;
    private Long cTime;
    private String img1;
    private String img2;
    private String img3;
    private String img4;
    private String img5;
    private String img6;

    public PostDTO4Create() {
    }

    public PostDTO4Create(Post post) {
        BeanUtils.copyProperties(post, this);
    }

    public Post toPost() {
        Post post = new Post();
        BeanUtils.copyProperties(this, post);
        return post;
    }
}
