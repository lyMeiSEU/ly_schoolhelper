package cn.edu.seu.alumni_server.common.config.service_fail_prompt;

import cn.edu.seu.alumni_server.service.fail.ActivityFailPrompt;
import cn.edu.seu.alumni_server.service.fail.ActivityMemberFailPrompt;
import cn.edu.seu.alumni_server.service.fail.AlumniCircleFailPrompt;
import java.io.IOException;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ServiceFailPromptConfig {

	@Bean
	public ActivityFailPrompt activityFailPrompt() throws IOException {
		return new ActivityFailPrompt("config/activity-fail-prompt.properties");
	}

	@Bean
	public ActivityMemberFailPrompt activityMemberFailPrompt() throws IOException {
		return new ActivityMemberFailPrompt("config/activity-member-fail-prompt.properties");
	}

	@Bean
	public AlumniCircleFailPrompt alumniCircleFailPrompt() throws IOException {
		return new AlumniCircleFailPrompt("config/alumni-circle-prompt.properties");
	}

}
