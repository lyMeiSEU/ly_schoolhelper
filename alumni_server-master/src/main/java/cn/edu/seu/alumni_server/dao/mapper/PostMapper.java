package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.internalRecommend.PostDTO4Return;
import cn.edu.seu.alumni_server.dao.entity.Post;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

import java.util.List;

@Repository
public interface PostMapper extends Mapper<Post> {
    List<PostDTO4Return> getPostList(Post post);
    PostDTO4Return getPost(Long postId);
}