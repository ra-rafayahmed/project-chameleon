import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import type { Story } from '@/data/mockData';

interface StoryCircleProps {
  story: Story;
  onClick: () => void;
}

export const StoryCircle = ({ story, onClick }: StoryCircleProps) => {
  return (
    <div 
      className="flex flex-col items-center gap-1 cursor-pointer group" 
      data-story-id={story.id}
      onClick={onClick}
    >
      <div
        className={`rounded-full p-[2px] bg-gradient-to-tr transition-all ${
          story.viewed
            ? 'from-muted to-muted'
            : 'from-gradient-start via-gradient-mid to-gradient-end'
        }`}
      >
        <div className="rounded-full p-[2px] bg-background">
          <Avatar className="h-16 w-16 group-hover:scale-105 transition-transform">
            <AvatarImage src={story.avatar} alt={story.username} />
            <AvatarFallback>{story.username[0].toUpperCase()}</AvatarFallback>
          </Avatar>
        </div>
      </div>
      <p
        className={`text-xs truncate w-16 text-center ${
          story.viewed ? 'text-muted-foreground' : 'text-foreground'
        }`}
      >
        {story.username}
      </p>
    </div>
  );
};
