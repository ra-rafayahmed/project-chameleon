import { useState } from 'react';
import { Navigation } from '@/components/Navigation';
import { BottomNavigation } from '@/components/BottomNavigation';
import { PostCard } from '@/components/PostCard';
import { StoryCircle } from '@/components/StoryCircle';
import { StoryViewer } from '@/components/StoryViewer';
import { CreatePostDialog } from '@/components/CreatePostDialog';
import { CreateStoryDialog } from '@/components/CreateStoryDialog';
import { usePosts, useStories, useCurrentUser } from '@/hooks/useLocalStorage';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Plus } from 'lucide-react';

const Home = () => {
  const { posts, addPost, likePost, addComment } = usePosts();
  const { stories, addStory, markAsViewed } = useStories();
  const { currentUser } = useCurrentUser();
  const [createPostOpen, setCreatePostOpen] = useState(false);
  const [createStoryOpen, setCreateStoryOpen] = useState(false);
  const [storyViewerOpen, setStoryViewerOpen] = useState(false);
  const [currentStoryIndex, setCurrentStoryIndex] = useState(0);

  const handleStoryClick = (storyId: string) => {
    const index = stories.findIndex(s => s.id === storyId);
    if (index !== -1) {
      setCurrentStoryIndex(index);
      setStoryViewerOpen(true);
    }
  };

  const handleNextStory = () => {
    if (currentStoryIndex < stories.length - 1) {
      setCurrentStoryIndex(prev => prev + 1);
    }
  };

  const handlePreviousStory = () => {
    if (currentStoryIndex > 0) {
      setCurrentStoryIndex(prev => prev - 1);
    }
  };

  const currentUserStory = stories.find(s => s.username === currentUser?.username);
  const otherStories = stories.filter(s => s.username !== currentUser?.username);

  return (
    <div className="min-h-screen bg-background pb-16">
      <Navigation />
      
      <main className="max-w-2xl mx-auto pt-16 pb-4 px-4">
        {/* Stories Section */}
        <section className="mb-8 overflow-x-auto">
          <div className="flex gap-4 py-4" data-section="stories">
            {/* Your Story */}
            {currentUser && (
              <div 
                className="flex flex-col items-center gap-1 cursor-pointer group" 
                onClick={() => setCreateStoryOpen(true)}
              >
                <div className="relative">
                  <div className="rounded-full p-[2px] bg-gradient-to-tr from-gradient-start via-gradient-mid to-gradient-end">
                    <div className="rounded-full p-[2px] bg-background">
                      <Avatar className="h-16 w-16 group-hover:scale-105 transition-transform">
                        <AvatarImage src={currentUser.avatar} alt={currentUser.username} />
                        <AvatarFallback>{currentUser.username[0].toUpperCase()}</AvatarFallback>
                      </Avatar>
                    </div>
                  </div>
                  <div className="absolute bottom-0 right-0 bg-primary rounded-full p-1 border-2 border-background">
                    <Plus className="h-3 w-3 text-primary-foreground" />
                  </div>
                </div>
                <p className="text-xs text-foreground text-center">Your Story</p>
              </div>
            )}
            
            {/* Other Stories */}
            {otherStories.map((story) => (
              <StoryCircle 
                key={story.id} 
                story={story} 
                onClick={() => handleStoryClick(story.id)}
              />
            ))}
          </div>
        </section>

        {/* Posts Feed */}
        <section className="space-y-6" data-section="feed">
          {posts.map((post) => (
            <PostCard 
              key={post.id} 
              post={post}
              onLike={likePost}
              onComment={addComment}
            />
          ))}
        </section>
      </main>

      <BottomNavigation onCreatePost={() => setCreatePostOpen(true)} />

      {/* Dialogs */}
      <CreatePostDialog
        open={createPostOpen}
        onOpenChange={setCreatePostOpen}
        onPostCreated={addPost}
      />

      <CreateStoryDialog
        open={createStoryOpen}
        onOpenChange={setCreateStoryOpen}
        onStoryCreated={addStory}
      />

      <StoryViewer
        story={stories[currentStoryIndex] || null}
        open={storyViewerOpen}
        onOpenChange={setStoryViewerOpen}
        onStoryViewed={markAsViewed}
        onNext={handleNextStory}
        onPrevious={handlePreviousStory}
        hasNext={currentStoryIndex < stories.length - 1}
        hasPrevious={currentStoryIndex > 0}
      />
    </div>
  );
};

export default Home;
